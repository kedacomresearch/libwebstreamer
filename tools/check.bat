@echo OFF
echo "XXXXXXXX"
set _FILTER=-whitespace/tab,-build/header_guard,-whitespace/braces,-readability/casting,-build/include_order,-build/include
for /r ..\lib %%h in (*.h) do (
   python cpplint.py --filter=%_FILTER% %%h
)
