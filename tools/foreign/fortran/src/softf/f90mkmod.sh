#!/usr/bin/env bash
# Fortran module dependency resolver.
# Writes make rule dependecies on standard output.
#
# Author:  Olaf Trygve Berglihn
# Email:   olafb@pvv.org
# Date:    2009-09-1
# Licence: GPLv3, see http://www.gnu.org/licenses/gpl-3.0.txt
#
# Usage: ./f90mkmod.sh *.f90 > moduledeps.mk
# Then include the moduledeps.mk at the end of your Makefile.

if [[ -z ${MODEXT} ]]; then MODEXT=mod; fi
if [[ -z ${OBJEXT} ]]; then OBJEXT=obj; fi


for f in $@
do
    objfile=$(basename $f | cut -d . -f1).${OBJEXT}
    containedmods=$(grep -i "^[[:space:]]*module" $f | 
    grep -vi "^[[:space:]]*module[[:space:]]*procedure" | 
    tr A-Z a-z | 
    sed "s/^[[:space:]]*module[[:space:]]*\\([a-z0-9_]*\\).*/\1/")
    for mod in ${containedmods}
    do
        modfile=${mod}.${MODEXT}
        echo -e "${modfile} : ${objfile}\n"
    done

    usedmods=$(grep -iP '^[[:space:]]*use(?!,[[:space:]]*intrinsic)[[[:space:]]|:]' ${f} |
    tr A-Z a-z  |
    sed "s/^[[:space:]]*use[[:space:]]*[^a-z0-9_]*\\([a-z0-9_]*\\).*/\\1.${MODEXT}/" |
    sort -u )
    if [[ -n ${usedmods} ]]
    then 
        echo -e "${objfile} : $(echo "${usedmods}" | tr '\n' ' ')\n"
    fi
done

