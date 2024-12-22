# mod_rexx
The original mod_rexx doesn't compile with regina-rexx. I made some adaptions and got it working on my Raspberry Pi Server.


## Things I did (just for me to remember)
```
REXXENTRY ersetzen:
find . -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i 's/REXXENTRY/APIENTRY/g' {} +

REXXPFN ersetzen:
find . -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i 's/REXXPFN/PFN/g' {} +

RexxReturnCode ersetzen:
find . -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i 's/RexxReturnCode/APIRET/g' {} +

CONSTRXSTRING ersetzen:
find . -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i 's/CONSTRXSTRING/RXSTRING/g' {} +

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
modrexx_error(...)
#pragma GCC diagnostic pop
```
