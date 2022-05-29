// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) 2017-2019 The Procyon Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PROCYON_QT_PROCYONADDRESSVALIDATOR_H
#define PROCYON_QT_PROCYONADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class ProcyonAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ProcyonAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Procyon address widget validator, checks for a valid procyon address.
 */
class ProcyonAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ProcyonAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // PROCYON_QT_PROCYONADDRESSVALIDATOR_H
