#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

PROCYOND=${PROCYOND:-$SRCDIR/procyond}
PROCYONCLI=${PROCYONCLI:-$SRCDIR/procyon-cli}
PROCYONTX=${PROCYONTX:-$SRCDIR/procyon-tx}
PROCYONQT=${PROCYONQT:-$SRCDIR/qt/procyon-qt}

[ ! -x $PROCYOND ] && echo "$PROCYOND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
RVNVER=($($PROCYONCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for procyond if --version-string is not set,
# but has different outcomes for procyon-qt and procyon-cli.
echo "[COPYRIGHT]" > footer.h2m
$PROCYOND --version | sed -n '1!p' >> footer.h2m

for cmd in $PROCYOND $PROCYONCLI $PROCYONTX $PROCYONQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${RVNVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${RVNVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
