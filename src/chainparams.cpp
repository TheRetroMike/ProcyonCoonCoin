// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2020 The RavenCoin Core developers
// Copyright (c) 2022-Present The Procyon Coon developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>
#include "chainparamsseeds.h"

//TODO: Take these out
extern double algoHashTotal[16];
extern int algoHashHits[16];


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The day raccoons were able to buy GPUs! - 25/05/2022.";
    const CScript genesisOutputScript = CScript() << ParseHex("87be734699ccc6a457d6208847af726ecdaa6f37954dfacfe7bc7d03744f973f6c3ea6706451813734596d01df160b3cb76abcfc4106269a9a48f55cda05b2955b") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

void CChainParams::TurnOffSegwit() {
	consensus.nSegwitEnabled = false;
}

void CChainParams::TurnOffCSV() {
	consensus.nCSVEnabled = false;
}

void CChainParams::TurnOffBIP34() {
	consensus.nBIP34Enabled = false;
}

void CChainParams::TurnOffBIP65() {
	consensus.nBIP65Enabled = false;
}

void CChainParams::TurnOffBIP66() {
	consensus.nBIP66Enabled = false;
}

bool CChainParams::BIP34() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP65() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP66() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::CSVEnabled() const{
	return consensus.nCSVEnabled;
}


/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 1051200;  //~ 3 yrs at 1.5 min block time
        consensus.nBIP34Enabled = false;
        consensus.nBIP65Enabled = false; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = false;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Estimated starting diff for first 180 kawpow blocks
        consensus.nPowTargetTimespan = 15 * 60; //15 min
        consensus.nPowTargetSpacing = 1.5 * 60;
		consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1613; // Approx 80% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 1814;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 2016;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;  //Assets (RIP2)
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1540944000; // Oct 31, 2018
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1572480000; // Oct 31, 2019
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 1814;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (RIP5)
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1578920400; // UTC: Mon Jan 13 2020 13:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1610542800; // UTC: Wed Jan 13 2021 13:00:00
        ///consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 1714; // Approx 85% of 2016
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1588788000; // UTC: Wed May 06 2020 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1620324000; // UTC: Thu May 06 2021 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 1714; // Approx 85% of 2016
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 2016;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1593453600; // UTC: Mon Jun 29 2020 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1624989600; // UTC: Mon Jun 29 2021 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 2016;
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1597341600; // UTC: Thu Aug 13 2020 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1628877600; // UTC: Fri Aug 13 2021 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 2016;


        // The best chain should have at least this much work
        //consensus.nMinimumChainWork = uint256S("000000000000000000000000000000000000000000000020d4ac871fb7009b63"); // Block 1186833

        // By default assume that the signatures in ancestors of this block are valid. Block# 1040000
        //consensus.defaultAssumeValid = uint256S("0x0000000000000d4840d4de1f7d943542c2aed532bd5d6527274fc0142fa1a410"); // Block 1186833

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x50; // P
        pchMessageStart[1] = 0x52; // R
        pchMessageStart[2] = 0x43; // C
        pchMessageStart[3] = 0x4F; // O
        nDefaultPort = 7333;
        nPruneAfterHeight = 100000;

        uint32_t nGenesisTime = 1653461735;  // 25/05/2022 08:55:35

        genesis = CreateGenesisBlock(nGenesisTime, 21161513, 0x1e00ffff, 4, 45 * COIN);

        consensus.hashGenesisBlock = genesis.GetX16RHash();

        assert(consensus.hashGenesisBlock == uint256S("0x000000653e8973d6062da09bb5aa194c30f62d14bb0d763d4a2b7d08637a3fbe"));
        assert(genesis.hashMerkleRoot == uint256S("10b952648efe215f957b9e5e3540c415623fcfd8cd92163f7c4b92adb04b4dc6"));

        vSeeds.emplace_back("149.102.158.171", false);
        vSeeds.emplace_back("85.208.51.135", false);

        // Address starts with P
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,55);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,122);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        // Procyon BIP44 cointype in mainnet is '8765'
        nExtCoinType = 8765;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
                { 0, uint256S("0x000000653e8973d6062da09bb5aa194c30f62d14bb0d763d4a2b7d08637a3fbe")}
            }
        };

        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Procyon chain
            // Stats as of 0x00000000000016ec03d8d93f9751323bcc42137b1b4df67e6a11c4394fd8e5ad window size 43200
            nGenesisTime, // * UNIX timestamp of last known number of transactions
            0,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.1       // * estimated number of transactions per second after that timestamp
        };

        /** PRCO Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 25 * COIN;
        nReissueAssetBurnAmount = 5 * COIN;
        nIssueSubAssetBurnAmount = 5 * COIN;
        nIssueUniqueAssetBurnAmount = .25 * COIN;
        nIssueMsgChannelAssetBurnAmount = 5 * COIN;
        nIssueQualifierAssetBurnAmount = 50 * COIN;
        nIssueSubQualifierAssetBurnAmount = 5 * COIN;
        nIssueRestrictedAssetBurnAmount = 75 * COIN;
        nAddNullQualifierTagBurnAmount = .005 * COIN;

        // Burn Addresses
        strIssueAssetBurnAddress = "PXissueAssetXXXXXXXXXXXXXXXXXhhZGt";
        strReissueAssetBurnAddress = "PXReissueAssetXXXXXXXXXXXXXXVEFAWu";
        strIssueSubAssetBurnAddress = "PXissueSubAssetXXXXXXXXXXXXXWcwhwL";
        strIssueUniqueAssetBurnAddress = "PXissueUniqueAssetXXXXXXXXXXWEAe58";
        strIssueMsgChannelAssetBurnAddress = "PXissueMsgChanneLAssetXXXXXXSjHvAY";
        strIssueQualifierAssetBurnAddress = "PXissueQuaLifierXXXXXXXXXXXXUgEDbC";
        strIssueSubQualifierAssetBurnAddress = "PXissueSubQuaLifierXXXXXXXXXVTzvv5";
        strIssueRestrictedAssetBurnAddress = "PXissueRestrictedXXXXXXXXXXXXzJZ1q";
        strAddNullQualifierTagBurnAddress = "PXaddTagBurnXXXXXXXXXXXXXXXXZQm5ya";

            //Global Burn Address
        strGlobalBurnAddress = "PXBurnXXXXXXXXXXXXXXXXXXXXXXWUo9FV";
 
        // DGW Activation
        nDGWActivationBlock = 1;

        nMaxReorganizationDepth = 60; // 60 at 1.5 minute block timespan is +/- 90 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 18 hours at 1.5 minute block

        nAssetActivationHeight = 1; // Asset activated block height
        nMessagingActivationBlock = 1; // Messaging activated block height
        nRestrictedActivationBlock = 1; // Restricted activated block height

        nKAAAWWWPOWActivationTime = 1653725030; 
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** RVN End **/
    }
};

/**
 * Testnet (v7)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 1051200;  //~ 3 yrs at 1.5 min block time
        consensus.nBIP34Enabled = false;
        consensus.nBIP65Enabled = false; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = false;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;

        consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1310; // Approx 65% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 1310;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 2016;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 5;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1533924000; // UTC: Fri Aug 10 2018 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1577257200; // UTC: Wed Dec 25 2019 07:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 1310;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 6;  //Assets (RIP5)
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1570428000; // UTC: Mon Oct 07 2019 06:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1577257200; // UTC: Wed Dec 25 2019 07:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 1310;
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1586973600; // UTC: Wed Apr 15 2020 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1618509600; // UTC: Thu Apr 15 2021 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 1310;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 2016;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1593453600; // UTC: Mon Jun 29 2020 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1624989600; // UTC: Mon Jun 29 2021 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 2016;
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1597341600; // UTC: Thu Aug 13 2020 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1628877600; // UTC: Fri Aug 13 2021 18:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 2016;

        // The best chain should have at least this much work.
        //consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000168050db560b4");

        // By default assume that the signatures in ancestors of this block are valid.
        //consensus.defaultAssumeValid = uint256S("0x000000006272208605c4df3b54d4d5515759105e7ffcb258e8cd8077924ffef1");


        pchMessageStart[0] = 0x50; // P
        pchMessageStart[1] = 0x52; // R
        pchMessageStart[2] = 0x43; // C
        pchMessageStart[3] = 0x4F; // O
        nDefaultPort = 17333;
        nPruneAfterHeight = 1000;

        uint32_t nGenesisTime = 1653464495;  // 25/05/2022 09:41:35

        // This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
//        /////////////////////////////////////////////////////////////////


//        arith_uint256 test;
//        bool fNegative;
//        bool fOverflow;
//        test.SetCompact(0x1e00ffff, &fNegative, &fOverflow);
//        std::cout << "Test threshold: " << test.GetHex() << "\n\n";
//
//        int genesisNonce = 0;
//        uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
//        uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
//        for (int i=0;i<40000000;i++) {
//            genesis = CreateGenesisBlock(nGenesisTime, i, 0x1e00ffff, 2, 5000 * COIN);
//            //genesis.hashPrevBlock = TempHashHolding;
//            // Depending on when the timestamp is on the genesis block. You will need to use GetX16RHash or GetX16RV2Hash. Replace GetHash() with these below
//            consensus.hashGenesisBlock = genesis.GetHash();
//
//            arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
//            if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
//                BestBlockHash = consensus.hashGenesisBlock;
//                std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
//                std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
//            }
//
//            TempHashHolding = consensus.hashGenesisBlock;
//
//            if (BestBlockHashArith < test) {
//                genesisNonce = i - 1;
//                break;
//            }
//            //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
//        }
//        std::cout << "\n";
//        std::cout << "\n";
//        std::cout << "\n";
//
//        std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
//        std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
//        std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
//
//        std::cout << "\n";
//        std::cout << "\n";
//        int totalHits = 0;
//        double totalTime = 0.0;
//
//        for(int x = 0; x < 16; x++) {
//            totalHits += algoHashHits[x];
//            totalTime += algoHashTotal[x];
//            std::cout << "hash algo " << x << " hits " << algoHashHits[x] << " total " << algoHashTotal[x] << " avg " << algoHashTotal[x]/algoHashHits[x] << std::endl;
//        }
//
//        std::cout << "Totals: hash algo " <<  " hits " << totalHits << " total " << totalTime << " avg " << totalTime/totalHits << std::endl;
//
//        genesis.hashPrevBlock = TempHashHolding;
//
//        return;

//        /////////////////////////////////////////////////////////////////

        genesis = CreateGenesisBlock(nGenesisTime, 22481531, 0x1e00ffff, 4, 45 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();

        //Test MerkleRoot and GenesisBlock
        assert(consensus.hashGenesisBlock == uint256S("0x000000ae3d893f753407f7439cd707d22e98671013f28894784c4857ae90087f"));
        assert(genesis.hashMerkleRoot == uint256S("10b952648efe215f957b9e5e3540c415623fcfd8cd92163f7c4b92adb04b4dc6"));

        vFixedSeeds.clear();
        vSeeds.clear();

        vSeeds.emplace_back("141.94.238.161", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Procyon BIP44 cointype in testnet
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
                    { 0, uint256S("0x000000ae3d893f753407f7439cd707d22e98671013f28894784c4857ae90087f")}
            }
        };

        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Procyon chain
            // Stats as of 00000023b66f46d74890287a7b1157dd780c7c5fdda2b561eb96684d2b39d62e window size 43200
            nGenesisTime, // * UNIX timestamp of last known number of transactions
            0,     // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.02        // * estimated number of transactions per second after that timestamp
        };

        /** PRCO Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 25 * COIN;
        nReissueAssetBurnAmount = 5 * COIN;
        nIssueSubAssetBurnAmount = 5 * COIN;
        nIssueUniqueAssetBurnAmount = .25 * COIN;
        nIssueMsgChannelAssetBurnAmount = 5 * COIN;
        nIssueQualifierAssetBurnAmount = 50 * COIN;
        nIssueSubQualifierAssetBurnAmount = 5 * COIN;
        nIssueRestrictedAssetBurnAmount = 75 * COIN;
        nAddNullQualifierTagBurnAmount = .005 * COIN;

        // Burn Addresses
        strIssueAssetBurnAddress = "n1issueAssetXXXXXXXXXXXXXXXXWdnemQ";
        strReissueAssetBurnAddress = "n1ReissueAssetXXXXXXXXXXXXXXWG9NLd";
        strIssueSubAssetBurnAddress = "n1issueSubAssetXXXXXXXXXXXXXbNiH6v";
        strIssueUniqueAssetBurnAddress = "n1issueUniqueAssetXXXXXXXXXXS4695i";
        strIssueMsgChannelAssetBurnAddress = "n1issueMsgChanneLAssetXXXXXXT2PBdD";
        strIssueQualifierAssetBurnAddress = "n1issueQuaLifierXXXXXXXXXXXXUysLTj";
        strIssueSubQualifierAssetBurnAddress = "n1issueSubQuaLifierXXXXXXXXXYffPLh";
        strIssueRestrictedAssetBurnAddress = "n1issueRestrictedXXXXXXXXXXXXZVT9V";
        strAddNullQualifierTagBurnAddress = "n1addTagBurnXXXXXXXXXXXXXXXXX5oLMH";

        // Global Burn Address
        strGlobalBurnAddress = "n1BurnXXXXXXXXXXXXXXXXXXXXXXU1qejP";

        // DGW Activation
        nDGWActivationBlock = 1;

        nMaxReorganizationDepth = 60; // 60 at 1.5 minute block timespan is +/- 90 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 18 hours

        nAssetActivationHeight = 6048; // Asset activated block height
        nMessagingActivationBlock = 10080; // Messaging activated block height
        nRestrictedActivationBlock = 10080; // Restricted activated block height

        nKAAAWWWPOWActivationTime = 1653661370; 
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** RVN End **/
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nBIP34Enabled = false;
        consensus.nBIP65Enabled = false; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = false;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.nSubsidyHalvingInterval = 150;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 108;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 144;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 0;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 999999999999ULL;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 144;
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (RIP5)
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 0; // GMT: Sun Mar 3, 2019 5:00:00 PM
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 999999999999ULL; // UTC: Wed Dec 25 2019 07:00:00
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
        //consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 144;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 0;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 999999999999ULL;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 208;
        //consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 288;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 0;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 999999999999ULL;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 108;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 144;
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 0;
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 999999999999ULL;
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 400;
        //consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 500;

        // The best chain should have at least this much work.
        //consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        //consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x50; // P
        pchMessageStart[1] = 0x52; // R
        pchMessageStart[2] = 0x43; // C
        pchMessageStart[3] = 0x4F; // O
        nDefaultPort = 27333;
        nPruneAfterHeight = 1000;

        uint32_t nGenesisTime = 1653464882;

// This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
//        /////////////////////////////////////////////////////////////////
//
//
//        arith_uint256 test;
//        bool fNegative;
//        bool fOverflow;
//        test.SetCompact(0x207fffff, &fNegative, &fOverflow);
//        std::cout << "Test threshold: " << test.GetHex() << "\n\n";
//
//        int genesisNonce = 0;
//        uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
//        uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
//        for (int i=0;i<40000000;i++) {
//            genesis = CreateGenesisBlock(1533751200, i, 0x207fffff, 2, 5000 * COIN);
//            //genesis.hashPrevBlock = TempHashHolding;
//            consensus.hashGenesisBlock = genesis.GetHash();
//
//            arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
//            if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
//                BestBlockHash = consensus.hashGenesisBlock;
//                std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
//                std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
//            }
//
//            TempHashHolding = consensus.hashGenesisBlock;
//
//            if (BestBlockHashArith < test) {
//                genesisNonce = i - 1;
//                break;
//            }
//            //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
//        }
//        std::cout << "\n";
//        std::cout << "\n";
//        std::cout << "\n";
//
//        std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
//        std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
//        std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
//
//        std::cout << "\n";
//        std::cout << "\n";
//        int totalHits = 0;
//        double totalTime = 0.0;
//
//        for(int x = 0; x < 16; x++) {
//            totalHits += algoHashHits[x];
//            totalTime += algoHashTotal[x];
//            std::cout << "hash algo " << x << " hits " << algoHashHits[x] << " total " << algoHashTotal[x] << " avg " << algoHashTotal[x]/algoHashHits[x] << std::endl;
//        }
//
//        std::cout << "Totals: hash algo " <<  " hits " << totalHits << " total " << totalTime << " avg " << totalTime/totalHits << std::endl;
//
//        genesis.hashPrevBlock = TempHashHolding;
//
//        return;

//        /////////////////////////////////////////////////////////////////


        genesis = CreateGenesisBlock(nGenesisTime, 823353, 0x207fffff, 4, 45 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();

        //assert(consensus.hashGenesisBlock == uint256S("0x00000fdd3626e8a409cc7e81f5b87389bb26276f8ac435745ade1df7515b6a8e"));
        //assert(genesis.hashMerkleRoot == uint256S("10b952648efe215f957b9e5e3540c415623fcfd8cd92163f7c4b92adb04b4dc6"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Procyon BIP44 cointype in regtest
        nExtCoinType = 1;

        /** PRCO Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 25 * COIN;
        nReissueAssetBurnAmount = 5 * COIN;
        nIssueSubAssetBurnAmount = 5 * COIN;
        nIssueUniqueAssetBurnAmount = .25 * COIN;
        nIssueMsgChannelAssetBurnAmount = 5 * COIN;
        nIssueQualifierAssetBurnAmount = 50 * COIN;
        nIssueSubQualifierAssetBurnAmount = 5 * COIN;
        nIssueRestrictedAssetBurnAmount = 75 * COIN;
        nAddNullQualifierTagBurnAmount = .005 * COIN;

        // Burn Addresses
        strIssueAssetBurnAddress = "n1issueAssetXXXXXXXXXXXXXXXXWdnemQ";
        strReissueAssetBurnAddress = "n1ReissueAssetXXXXXXXXXXXXXXWG9NLd";
        strIssueSubAssetBurnAddress = "n1issueSubAssetXXXXXXXXXXXXXbNiH6v";
        strIssueUniqueAssetBurnAddress = "n1issueUniqueAssetXXXXXXXXXXS4695i";
        strIssueMsgChannelAssetBurnAddress = "n1issueMsgChanneLAssetXXXXXXT2PBdD";
        strIssueQualifierAssetBurnAddress = "n1issueQuaLifierXXXXXXXXXXXXUysLTj";
        strIssueSubQualifierAssetBurnAddress = "n1issueSubQuaLifierXXXXXXXXXYffPLh";
        strIssueRestrictedAssetBurnAddress = "n1issueRestrictedXXXXXXXXXXXXZVT9V";
        strAddNullQualifierTagBurnAddress = "n1addTagBurnXXXXXXXXXXXXXXXXX5oLMH";

        // Global Burn Address
        strGlobalBurnAddress = "n1BurnXXXXXXXXXXXXXXXXXXXXXXU1qejP";

        // DGW Activation
        nDGWActivationBlock = 1;

        nMaxReorganizationDepth = 60; // 60 at 1.5 minute block timespan is +/- 90 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 18 hours

        nAssetActivationHeight = 0; // Asset activated block height
        nMessagingActivationBlock = 0; // Messaging activated block height
        nRestrictedActivationBlock = 0; // Restricted activated block height

        // TODO, we need to figure out what to do with this for regtest. This effects the unit tests
        // For now we can use a timestamp very far away
        // If you are looking to test the kawpow hashing function in regtest. You will need to change this number
        nKAAAWWWPOWActivationTime = 1653661370;
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** RVN End **/
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &GetParams() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network, bool fForceBlockNetwork)
{
    SelectBaseParams(network);
    if (fForceBlockNetwork) {
        bNetwork.SetNetwork(network);
    }
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}

void TurnOffSegwit(){
	globalChainParams->TurnOffSegwit();
}

void TurnOffCSV() {
	globalChainParams->TurnOffCSV();
}

void TurnOffBIP34() {
	globalChainParams->TurnOffBIP34();
}

void TurnOffBIP65() {
	globalChainParams->TurnOffBIP65();
}

void TurnOffBIP66() {
	globalChainParams->TurnOffBIP66();
}
