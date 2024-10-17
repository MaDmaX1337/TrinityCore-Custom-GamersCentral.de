#include "AccountMgr.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Common.h"
#include "Chat.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SharedDefines.h"
#include "World.h"
#include "WorldSession.h"
#include "Spells/SpellMgr.h"
#include <Commands/cs_learn.cpp>
class Trinitycore_Paymaster_announce : public PlayerScript
{
public:
    Trinitycore_Paymaster_announce() : PlayerScript("Trinitycore_Paymaster_announce") { }
    void OnLogin(Player* Player, bool /*firstLogin*/) override
    {
        if (sConfigMgr->GetBoolDefault("Paymaster.Announce.Enable", true))
        {
            ChatHandler(Player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Paymaster NPC");
        }
    }
};
class npc_paymaster : public CreatureScript
{
public:
    npc_paymaster() : CreatureScript("npc_paymaster") { }
    struct npc_paymasterAI : public ScriptedAI
    {
        npc_paymasterAI(Creature* creature) : ScriptedAI(creature) { }
        bool OnGossipHello(Player* player) override
        {
            if (player->IsInCombat())
            {
                ClearGossipMenuFor(player);
                ChatHandler(player->GetSession()).PSendSysMessage("You are still in combat!");
            }
            else
            {
                const int32 addGold = sConfigMgr->GetIntDefault("Paymaster.AddGold", 5000);
                std::ostringstream goldMessage;
                goldMessage << addGold << " gold";
                AddGossipItemFor(player, GossipOptionNpc::None, goldMessage.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                AddGossipItemFor(player, GossipOptionNpc::None, "Profession Trainer", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                AddGossipItemFor(player, GossipOptionNpc::None, "I want to be Aldor.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                AddGossipItemFor(player, GossipOptionNpc::None, "I want to be Scryer.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                AddGossipItemFor(player, GossipOptionNpc::None, "I want Outland Reputation", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                AddGossipItemFor(player, GossipOptionNpc::None, "I want to align with the Frenzyheart Tribe.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                AddGossipItemFor(player, GossipOptionNpc::None, "I want to align with the Oracles.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                AddGossipItemFor(player, GossipOptionNpc::None, "I want Northrend Reputation", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
                SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());
            }
            return true;
        }
        bool OnGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            if (action > GOSSIP_ACTION_INFO_DEF + 4 && player->GetFreePrimaryProfessionPoints() == 0)
                return false;
            const int32 addGold = sConfigMgr->GetIntDefault("Paymaster.AddGold", 5000);
            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF:
                player->ModifyMoney(addGold*10000);
                break;
            case GOSSIP_ACTION_INFO_DEF + 1:
                ClearGossipMenuFor(player);
                AddGossipItemFor(player, GossipOptionNpc::None, "Jewelcrafting", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20);
                AddGossipItemFor(player, GossipOptionNpc::None, "Inscription", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
                AddGossipItemFor(player, GossipOptionNpc::None, "Alchemy", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
                AddGossipItemFor(player, GossipOptionNpc::None, "Blacksmithing", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 23);
                AddGossipItemFor(player, GossipOptionNpc::None, "Enchanting", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 24);
                AddGossipItemFor(player, GossipOptionNpc::None, "Engineering", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 25);
                AddGossipItemFor(player, GossipOptionNpc::None, "Herbalism", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 26);
                AddGossipItemFor(player, GossipOptionNpc::None, "Leatherworking", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 27);
                AddGossipItemFor(player, GossipOptionNpc::None, "Mining", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 28);
                AddGossipItemFor(player, GossipOptionNpc::None, "Skinning", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 29);
                AddGossipItemFor(player, GossipOptionNpc::None, "Tailoring", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30);
                AddGossipItemFor(player, GossipOptionNpc::None, "First Aid", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 31);
                SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());
                return true;
            case GOSSIP_ACTION_INFO_DEF + 2: // aldor
                player->SetReputation(932, 84000);
                break;
            case GOSSIP_ACTION_INFO_DEF + 3: // scryer
                player->SetReputation(934, 84000);
                break;
            case GOSSIP_ACTION_INFO_DEF + 4: // outland misc
                player->SetReputation(933, 84000); // consortium
                player->SetReputation(935, 84000); // shatar
                player->SetReputation(942, 84000); // cenarion               
                player->SetReputation(967, 84000); // violet eye
                player->SetReputation(970, 84000); // sporegar
                player->SetReputation(989, 84000); // keepers of time
                player->SetReputation(990, 84000); // scale of sands
                player->SetReputation(1011, 84000); // lower city
                player->SetReputation(1012, 84000); // ashtounge
                player->SetReputation(1015, 84000); // netherwing
                player->SetReputation(1031, 84000); // Skyguard
                player->SetReputation(1038, 84000); // ogrila
                player->SetReputation(1077, 84000); // shattered sun
                if (player->GetFaction() == ALLIANCE)
                {
                    player->SetReputation(946, 84000); // honor hold
                    player->SetReputation(978, 84000); // kureni
                }
                else
                {
                    player->SetReputation(947, 84000); // thrallmar
                    player->SetReputation(941, 84000); // mag'har
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 5: 
                player->SetReputation(1104, 84000); // frenzyheart tribe
                break;
            case GOSSIP_ACTION_INFO_DEF + 6: 
                player->SetReputation(1105, 84000); // oracles
                break; 
            case GOSSIP_ACTION_INFO_DEF + 7: // northrend misc
                player->SetReputation(1073, 84000); // kaluk
                player->SetReputation(1090, 84000); // kirin tor
                player->SetReputation(1091, 84000); // wyrmrest accord
                player->SetReputation(1098, 84000); // ebon blade
                player->SetReputation(1106, 84000); // argent crusade
                player->SetReputation(1119, 84000); // sons of hodir
                player->SetReputation(1156, 84000); // ashen verdict
                if (player->GetFaction() == ALLIANCE)
                {
                    player->SetReputation(1037, 84000); // alliance vanguard
                    player->SetReputation(1050, 84000); // valiance expedition
                    player->SetReputation(1068, 84000); // explorers league
                    player->SetReputation(1094, 84000); // silver covenant
                }
                else
                {
                    player->SetReputation(1052, 84000); // horde expedition
                    player->SetReputation(1064, 84000); // taunka
                    player->SetReputation(1067, 84000); // hand of vengeance
                    player->SetReputation(1085, 84000); // warsong offensive
                    player->SetReputation(1124, 84000); // sunreavers
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 20: // jc
                player->LearnSpell(51311, true);
                player->SetSkill(755, 0, 450, 450);
                learn_commandscript::HandleLearnSkillRecipesHelper(player, 755);
                break;
            case GOSSIP_ACTION_INFO_DEF + 21: // inscription
                player->LearnSpell(45363, true);
                player->SetSkill(773, 0, 450, 450);
                learn_commandscript::HandleLearnSkillRecipesHelper(player, 773);
                break;
            case GOSSIP_ACTION_INFO_DEF + 22: // alchemy
                player->LearnSpell(51304, true);
                player->SetSkill(171, 0, 450, 450);
                learn_commandscript::HandleLearnSkillRecipesHelper(player, 171);
                break;
            case GOSSIP_ACTION_INFO_DEF + 23: // blacksmithing
                player->LearnSpell(51300, true);
                player->SetSkill(164, 0, 450, 450);
                learn_commandscript::HandleLearnSkillRecipesHelper(player, 164);
                break;
            case GOSSIP_ACTION_INFO_DEF + 24: // enchanting
                player->LearnSpell(51313, true);
                player->SetSkill(333, 0, 450, 450);
                learn_commandscript::HandleLearnSkillRecipesHelper(player, 333);
                break;
            case GOSSIP_ACTION_INFO_DEF + 25: // engineering
                player->LearnSpell(51306, true);
                player->SetSkill(202, 0, 450, 450);
                learn_commandscript::HandleLearnSkillRecipesHelper(player, 202);
                break;
            case GOSSIP_ACTION_INFO_DEF + 26: // herbalism
                player->LearnSpell(50300, true);
                player->SetSkill(182, 0, 450, 450);
                learn_commandscript::HandleLearnSkillRecipesHelper(player, 182);
                break;
            case GOSSIP_ACTION_INFO_DEF + 27: // leatherworking
                player->LearnSpell(51302, true);
                player->SetSkill(165, 0, 450, 450);
                learn_commandscript::HandleLearnSkillRecipesHelper(player, 165);
                break;
            case GOSSIP_ACTION_INFO_DEF + 28: // mining
                player->LearnSpell(50310, true);
                player->SetSkill(186, 0, 450, 450);
                learn_commandscript::HandleLearnSkillRecipesHelper(player, 186);
                break;
            case GOSSIP_ACTION_INFO_DEF + 29: // skinning
                player->LearnSpell(50305, true);
                player->SetSkill(393, 0, 450, 450);
                learn_commandscript::HandleLearnSkillRecipesHelper(player, 393);
                break;
            case GOSSIP_ACTION_INFO_DEF + 30: // tailoring
                player->LearnSpell(51309, true);
                player->SetSkill(197, 0, 450, 450);
                learn_commandscript::HandleLearnSkillRecipesHelper(player, 197);
                break;
            case GOSSIP_ACTION_INFO_DEF + 31: // first aid
                player->LearnSpell(45542, true);
                player->SetSkill(129, 0, 450, 450);
                learn_commandscript::HandleLearnSkillRecipesHelper(player, 129);
                break;
            
            default:
                return false;
            }
            CloseGossipMenuFor(player);
            return true;
        }
    };
    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_paymasterAI(creature);
    }
};
void AddSC_paymaster_npc()
{
    new Trinitycore_Paymaster_announce();
    new npc_paymaster();
}
