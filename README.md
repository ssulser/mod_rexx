# mod_rexx
The original mod_rexx doesn't compile with regina-rexx. I made some adaptions and got it working on my Raspberry Pi Server.


## Things I did (just for me to remember)
```
replace REXXENTRY by APIENTRY:
find . -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i 's/REXXENTRY/APIENTRY/g' {} +

replace REXXPFN by PFN:
find . -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i 's/REXXPFN/PFN/g' {} +

replace RexxReturnCode by APIRET:
find . -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i 's/RexxReturnCode/APIRET/g' {} +

replace CONSTRXSTRING by RXSTRING:
find . -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i 's/CONSTRXSTRING/RXSTRING/g' {} +

enclose every call to modrexx_error() with pragmas:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
modrexx_error(...)
#pragma GCC diagnostic pop

start compiling with rexx make_mod_rexx.rex regina
- but before that makefile.linux needs some changes
- path to apxs is wrong with an apache standard installation
- $(DEFINES) added to call of apxs so USE_REGINA ist applied

move compiled & linked mod_rexx.so to /usr/lib/apache2/modules/ (set it in rexx.load)

move rspcomp.rex to /usr/local/bin (set it in rexx.conf)

rexx.conf has to be split into two files:
- rexx.conf
- rexx.load
- both go into /etc/apache2/mods-available
- activate it with sudo a2enmod rexx & restart apache2
```
