@echo OFF
echo "XXXXXXXX"
pushd tools
set _FILTER=-whitespace/tab,-build/header_guard,-whitespace/braces,-readability/casting,-build/include_order,-build/include
for /r ..\lib %%h in (*.h) do (
   python cpplint.py --filter=%_FILTER% --linelength=120 %%h 
)
for /r ..\lib %%h in (*.cc) do (
   python cpplint.py --filter=%_FILTER% --linelength=120 %%h 
)
popd