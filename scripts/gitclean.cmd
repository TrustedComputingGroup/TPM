@echo off
git clean -ndx --exclude .vscode --exclude TPMCmd/.vs

echo Press Ctrl+C NOW if you don't want to delete these.
pause

git clean -fdx --exclude .vscode --exclude TPMCmd/.vs
