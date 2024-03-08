@echo off
rem format files staged in the .git index
for /f %%i in ('git diff-index --cached --name-only HEAD ^| findstr /IR /C:\.h /C:\.c /C:\.cpp /C:\.hpp /C:\.inl ^| findstr /v /C:\.cmd') do @clang-format --style=file -i --verbose %%i
