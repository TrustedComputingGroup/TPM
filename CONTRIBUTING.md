# How to contribute
- [Guidelines for reporting bugs](#guidelines-for-reporting-bugs)
- [Guideline for submitting changes](#guideline-for-submitting-changes)
- [Contributing](#contributing)
- [Submitting a bug fix](#submitting-a-bug-fix)

---
## Guidelines for reporting bugs

See [SECURITY.md](SECURITY.md) for guidance on reporting security bugs to TCG.

Non-security-critical bugs can be filed on the "Issues" tracker for this
repository.

## Guideline for submitting changes

This repository tracks official TPM Library Specification releases and errata from
the Trusted Computing Group:

https://trustedcomputinggroup.org/tpm-library-specification/

All changes to core TPM logic, particularly changes to files in
TPMCmd/tpm and its subdirectories, must be approved by TCG voting
members. Github pull requests may be used to propose changes, but changes
will not be incorporated without TCG member approval.

Other changes (e.g. new files or changes to TPMCmd/Platform or TPMCmd/Simulator),
particularly to support new platforms, scenarios, build environments or
crypto-libraries, will be considered if they are expected to be widely useful.

Contributors that wish to be involved in
the future evolution of the TPM specification and reference implementation
should consider joining the Trusted Computing Group. Information about
membership and liaison programs is available at https://trustedcomputinggroup.org/membership/

## Contributing

While all normative changes to the behavior of the TPM are made within Trusted
Computing Group, this repository welcomes (non-security) bugfixes.

Contributions are subject to the repository [license](LICENSE).

## Submitting a bug fix

If you are new, please read the documentation beginning with [Introduction](docs/architecture/introduction.md)

The following are prerequisites _*before*_ requesting change approval:
- Decide if the change is a feature or a bug fix and describe it accordingly
- Get a solid understanding of the change and the scope
- Decide specifically which versions of the TPM code need to be updated

> Make sure you are familiar with the architecture of the TPM codebase

NOTE: If introducing multiple changes, treat each change individually creating a different branch and pull request for each change

This will make the code review (acceptance) process much easier
