#ifndef PROCYON_QT_TEST_WALLETTESTS_H
#define PROCYON_QT_TEST_WALLETTESTS_H

#include <QObject>
#include <QTest>

class WalletTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void walletTests();
};

#endif // PROCYON_QT_TEST_WALLETTESTS_H
