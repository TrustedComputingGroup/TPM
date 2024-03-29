#include "Tpm.h"
#include "CreateLoaded_fp.h"

#if CC_CreateLoaded  // Conditional expansion of this file

/*(See part 3 of specification)
 * Create and load any type of key, including a temporary key.
 * The input template is a marshaled public area rather than an unmarshaled one as
 * used in Create and CreatePrimary. This is so that the label and context that
 * could be in the template can be processed without changing the formats for the
 * calls to Create and CreatePrimary.
*/
//  Return Type: TPM_RC
//      TPM_RC_ATTRIBUTES       'sensitiveDataOrigin' is CLEAR when 'sensitive.data'
//                              is an Empty Buffer;
//                              'fixedTPM', 'fixedParent', or 'encryptedDuplication'
//                              attributes are inconsistent between themselves or with
//                              those of the parent object;
//                              inconsistent 'restricted', 'decrypt' and 'sign'
//                              attributes;
//                              attempt to inject sensitive data for an asymmetric
//                              key;
//                              attempt to create a symmetric cipher key that is not
//                              a decryption key
//      TPM_RC_FW_LIMITED       The requested hierarchy is FW-limited, but the TPM
//                              does not support FW-limited objects or the TPM failed
//                              to derive the Firmware Secret.
//      TPM_RC_SVN_LIMITED      The requested hierarchy is SVN-limited, but the TPM
//                              does not support SVN-limited objects or the TPM failed
//                              to derive the Firmware SVN Secret for the requested
//                              SVN.
//      TPM_RC_KDF              incorrect KDF specified for decrypting keyed hash
//                              object
//      TPM_RC_KEY              the value of a provided symmetric key is not allowed
//      TPM_RC_OBJECT_MEMORY    there is no free slot for the object
//      TPM_RC_SCHEME           inconsistent attributes 'decrypt', 'sign',
//                              'restricted' and key's scheme ID; or hash algorithm is
//                              inconsistent with the scheme ID for keyed hash object
//      TPM_RC_SIZE             size of public authorization policy or sensitive
//                              authorization value does not match digest size of the
//                              name algorithm sensitive data size for the keyed hash
//                              object is larger than is allowed for the scheme
//      TPM_RC_SYMMETRIC        a storage key with no symmetric algorithm specified;
//                              or non-storage key with symmetric algorithm different
//                              from TPM_ALG_NULL
//      TPM_RC_TYPE             cannot create the object of the indicated type
//                              (usually only occurs if trying to derive an RSA key).
TPM_RC
TPM2_CreateLoaded(CreateLoaded_In*  in,  // IN: input parameter list
                  CreateLoaded_Out* out  // OUT: output parameter list
)
{
    TPM_RC       result = TPM_RC_SUCCESS;
    OBJECT*      parent = HandleToObject(in->parentHandle);
    OBJECT*      newObject;
    BOOL         derivation;
    TPMT_PUBLIC* publicArea;
    RAND_STATE   randState;
    RAND_STATE*  rand = &randState;
    TPMS_DERIVE  labelContext;

    // Input Validation

    // How the public area is unmarshaled is determined by the parent, so
    // see if parent is a derivation parent
    derivation = (parent != NULL && parent->attributes.derivation);

    // If the parent is an object, then make sure that it is either a parent or
    // derivation parent
    if(parent != NULL && !parent->attributes.isParent && !derivation)
        return TPM_RCS_TYPE + RC_CreateLoaded_parentHandle;

    // Get a spot in which to create the newObject
    newObject = FindEmptyObjectSlot(&out->objectHandle);
    if(newObject == NULL)
        return TPM_RC_OBJECT_MEMORY;

    // Do this to save typing
    publicArea = &newObject->publicArea;

    // Unmarshal the template into the object space. TPM2_Create() and
    // TPM2_CreatePrimary() have the publicArea unmarshaled by CommandDispatcher.
    // This command is different because of an unfortunate property of the
    // unique field of an ECC key. It is a structure rather than a single TPM2B. If
    // if had been a TPM2B, then the label and context could be within a TPM2B and
    // unmarshaled like other public areas. Since it is not, this command needs its
    // on template that is a TPM2B that is unmarshaled as a BYTE array with a
    // its own unmarshal function.
    result = UnmarshalToPublic(publicArea, &in->inPublic, derivation, &labelContext);
    if(result != TPM_RC_SUCCESS)
        return result + RC_CreateLoaded_inPublic;

    // Validate that the authorization size is appropriate
    if(!AdjustAuthSize(&in->inSensitive.sensitive.userAuth, publicArea->nameAlg))
        return TPM_RCS_SIZE + RC_CreateLoaded_inSensitive;

    // Command output
    if(derivation)
    {
        TPMT_KEYEDHASH_SCHEME* scheme;
        scheme = &parent->publicArea.parameters.keyedHashDetail.scheme;

        // SP800-108 is the only KDF supported by this implementation and there is
        // no default hash algorithm.
        pAssert(scheme->details.xor.hashAlg != TPM_ALG_NULL
                && scheme->details.xor.kdf == TPM_ALG_KDF1_SP800_108);
        // Don't derive RSA keys
        if(publicArea->type == TPM_ALG_RSA)
            return TPM_RCS_TYPE + RC_CreateLoaded_inPublic;
        // sensitiveDataOrigin has to be CLEAR in a derived object. Since this
        // is specific to a derived object, it is checked here.
        if(IS_ATTRIBUTE(
               publicArea->objectAttributes, TPMA_OBJECT, sensitiveDataOrigin))
            return TPM_RCS_ATTRIBUTES;
        // Check the rest of the attributes
        result = PublicAttributesValidation(parent, 0, publicArea);
        if(result != TPM_RC_SUCCESS)
            return RcSafeAddToResult(result, RC_CreateLoaded_inPublic);
        // Process the template and sensitive areas to get the actual 'label' and
        // 'context' values to be used for this derivation.
        result = SetLabelAndContext(&labelContext, &in->inSensitive.sensitive.data);
        if(result != TPM_RC_SUCCESS)
            return result;
        // Set up the KDF for object generation
        DRBG_InstantiateSeededKdf((KDF_STATE*)rand,
                                  scheme->details.xor.hashAlg,
                                  scheme->details.xor.kdf,
                                  &parent->sensitive.sensitive.bits.b,
                                  &labelContext.label.b,
                                  &labelContext.context.b,
                                  TPM_MAX_DERIVATION_BITS);
        // Clear the sensitive size so that the creation functions will not try
        // to use this value.
        in->inSensitive.sensitive.data.t.size = 0;
    }
    else
    {
        // Check attributes in input public area. CreateChecks() checks the things
        // that are unique to creation and then validates the attributes and values
        // that are common to create and load.
        result = CreateChecks(parent,
                              (parent == NULL) ? in->parentHandle : 0,
                              publicArea,
                              in->inSensitive.sensitive.data.t.size);

        if(result != TPM_RC_SUCCESS)
            return RcSafeAddToResult(result, RC_CreateLoaded_inPublic);
        // Creating a primary object
        if(parent == NULL)
        {
            TPM2B_NAME name;
            TPM2B_SEED primary_seed;

            newObject->attributes.primary = SET;
            if(HierarchyNormalizeHandle(in->parentHandle) == TPM_RH_ENDORSEMENT)
                newObject->attributes.epsHierarchy = SET;

            result = HierarchyGetPrimarySeed(in->parentHandle, &primary_seed);
            if(result != TPM_RC_SUCCESS)
                return result;

            // If so, use the primary seed and the digest of the template
            // to seed the DRBG
            result = DRBG_InstantiateSeeded(
                (DRBG_STATE*)rand,
                &primary_seed.b,
                PRIMARY_OBJECT_CREATION,
                (TPM2B*)PublicMarshalAndComputeName(publicArea, &name),
                &in->inSensitive.sensitive.data.b);
            MemorySet(primary_seed.b.buffer, 0, primary_seed.b.size);

            if(result != TPM_RC_SUCCESS)
                return result;
        }
        else
        {
            // This is an ordinary object so use the normal random number generator
            rand = NULL;
        }
    }
    // Internal data update
    // Create the object
    result = CryptCreateObject(newObject, &in->inSensitive.sensitive, rand);
    DRBG_Uninstantiate((DRBG_STATE*)rand);
    if(result != TPM_RC_SUCCESS)
        return result;
    // if this is not a Primary key and not a derived key, then return the sensitive
    // area
    if(parent != NULL && !derivation)
        // Prepare output private data from sensitive
        SensitiveToPrivate(&newObject->sensitive,
                           &newObject->name,
                           parent,
                           newObject->publicArea.nameAlg,
                           &out->outPrivate);
    else
        out->outPrivate.t.size = 0;
    // Set the remaining return values
    out->outPublic.publicArea = newObject->publicArea;
    out->name                 = newObject->name;
    // Set the remaining attributes for a loaded object
    ObjectSetLoadedAttributes(newObject, in->parentHandle);

    return result;
}

#endif  // CC_CreateLoaded