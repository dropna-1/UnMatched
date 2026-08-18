#include <algorithm>
#include <queue>
#include "Game/Characters/SideKick.hpp"
#include "Game/Game.hpp"
#include "Game/Cards/Deck.hpp"
#include "Game/Pending/Pending.hpp"
#include "Game/Factory/CardFactory.hpp"
#include "Game/Common/SaveManager.hpp"
using namespace std;


Game::Game() : dracula(HeroFactory::createDracula())
, sherlock(HeroFactory::createSherlock()), invisible(HeroFactory::createInvisibleMan()){}


shared_ptr<Hero>& Game::getDracula(){
    return dracula;
}

unique_ptr<PendingCombat>& Game::getPendingCombat(){
    return pendingCombat;
}

void Game::clearPendingCombat(){
    auto& s = pendingCombat->selection;
    s.cards.clear();
    s.character = nullptr;
    s.destination = -1;
    s.showHand = false;
    s.canFinish = false;
    pendingCombat.reset();
}


void Game::setPlayer1(const string& name, const int& age){
    player1.setName(name);
    player1.setAge(age);
}


void Game::setPlayer2(const string& name, const int& age){
    player2.setName(name);
    player2.setAge(age);
}

void Game::setCanUseAbility(const bool& use){
    canUseAbility = use;
}

void Game::setupPlayers(){
    if(player1.getAge() < player2.getAge()){
        currentPlayer = &player1;
        otherPlayer = &player2;
    }
    else if(player1.getAge() > player2.getAge()){
        otherPlayer = &player1;
        currentPlayer = &player2;
    }
    else{
        std::uniform_int_distribution<int> dist(0, 1);
        if (dist(rng) == 0) {
            currentPlayer = &player1;
            otherPlayer = &player2;
        } else {
            currentPlayer = &player2;
            otherPlayer = &player1;
        }
    }
}


Player* Game::getCurrentPlayer(){
    return currentPlayer;
}


Player* Game::getOtherPlayer(){
    return otherPlayer;
}


void Game::choiceHero(Player& player, HeroType choice){
    player.setHero(
        (choice == dracula.get()->getHeroType() ? dracula : 
        (choice == sherlock.get()->getHeroType() ? sherlock : invisible))
    );
}


void Game::setupGame(){
    currentPlayer->getHero().get()->setPosition(18);
    otherPlayer->getHero().get()->setPosition(4);
    for (int i = 0; i < 5; i++) {
        currentPlayer->getHero()->getDeck()->drawCard();
        otherPlayer->getHero()->getDeck()->drawCard();
    }
}


void Game::resetAction(){
    actionsRemaining = 2;
}


Board& Game::getBoard(){
    return board;
}


const vector<shared_ptr<Card>>& Game::showOtherHand(){
    return otherPlayer->getHero().get()->getDeck().get()->getHand();
}


void Game::useAction(){
    if(currentPlayer->getHero()->getDeck()->getHandSize() > 7){
        requestAction(make_unique<DeleteCardAction>(*this, currentPlayer));
        return;
    }
    actionsRemaining--;
    if(actionsRemaining == 0){
        nextTurn();
    }
}

void Game::nextTurn(){
    changeTurn();
    resetAction();
}

int Game::getRemainingActions() const{
    return actionsRemaining;
}


vector<int> Game::getAvailableMoves(Character* character, 
    const int& spacing)
{
    vector<int> reachable;
    queue<pair<int, int>> q;
    vector<bool> visited(board.size(), false);

    int start = character->getPosition();
    q.push({start, 0});
    visited[start] = true;

    while(!q.empty()){
        auto current = q.front();
        q.pop();

        int place = current.first;
        int dist = current.second;

        if(dist == spacing)
            continue;

        vector<int> neigh = board.getSpace(place).neighbors;
        if(!board.getSpace(place).secret.empty())
            for(int secret : board.getSpace(place).secret)
                neigh.push_back(secret);

        for(int next : neigh){
            bool enemy = false;
            bool dom = false;

            if(visited[next])
                continue;

            for(Character* c : otherPlayer->getAllCharacters())
                if(next == c->getPosition()){
                    enemy = true;
                    break;
                }
            
            if(enemy)
                continue;

            for(Character* c : currentPlayer->getAllCharacters())
                if(next == c->getPosition()){
                    dom = true;
                    break;
                }

            visited[next] = true;
            q.push({next, dist+1});
  
            if(!dom)
                reachable.push_back(next);
        }
    }
    sort(reachable.begin(), reachable.end());
    return reachable;
}


vector<int> Game::getAllSpaces(){
    vector<int> allSpaces;
    for(int space = 0; space < 32; space++)
        if(canMove(space))
            allSpaces.push_back(space); 
    return allSpaces;
}

vector<int> Game::getFreeSpacesNearby(Character* character){
    vector<int> freeSpaces;
    for(int neigh : board.getSpace(character->getPosition()).neighbors)
        if(canMove(neigh))
            freeSpaces.push_back(neigh);
    return freeSpaces;
}

bool Game::canMove(int to) const{
    for(auto character : currentPlayer->getAllCharacters())
        if(character->getPosition() == to)
            return false;
    for(auto character : otherPlayer->getAllCharacters())
        if(character->getPosition() == to)
            return false;
    return true;
}


void Game::move(Character* character, const int& pos){
    character->setPosition(pos);
}


bool Game::canManever(){
    for(Character* c : currentPlayer->getAllCharacters())
        if(!getAvailableMoves(c, c->getMovement()).empty())
            return true;
    return false;
}


void Game::performManeuver(Character* character, const int& pos){
    move(character, pos);
    currentPlayer->getHero().get()->getDeck().get()->drawCard();
    useAction();
}



void Game::requestAction(unique_ptr<PendingAction> action){
    pendingActions.push_back(std::move(action));
}


bool Game::hasPendingAction() const{
    return !pendingActions.empty();
}


PendingAction* Game::currentPendingAction(){
    if(pendingActions.empty())
        return nullptr;
    return pendingActions.front().get();
}


void Game::completePendingAction(){
    pendingActions.pop_front();
}


int Game::calculateDamage(Card* attack, Card* defense){
    if(defense == nullptr)
        return attack->getValue();
    if(attack->getValue() > defense->getValue())
        return attack->getValue() - defense->getValue();
    return 0;
}


vector<Character*> Game::getEnemiesNearby(){
    vector<Character*> enemies;
    auto neighboors = board.getSpace(dracula.get()->getPosition()).neighbors;
    for(int target : neighboors){
        for(auto character : player1.getAllCharacters())
            if(character->getPosition() == target)
                enemies.push_back(character);
        for(auto character : player2.getAllCharacters())
            if(character->getPosition() == target)
                enemies.push_back(character);
    }
    return enemies;
}

vector<int> Game::getPlayableAttackCard(Character* attacker)
{
    vector<int> playableCards;
    bool ishero = attacker->isHero();
    auto ahand = currentPlayer->getHero().get()->getDeck().get()->getHand();
    for(int card = 0; card < ahand.size(); card++)

        if(ahand.at(card)->getType() == CardType::Attack || 
        ahand.at(card)->getType() == CardType::Versalite)
        {
            if(ahand.at(card)->getFighter() == FighterType::Any)
                playableCards.push_back(card);
            else if(ishero && ahand.at(card)->getFighter() == FighterType::Hero)
                playableCards.push_back(card);
            else if(!ishero && ahand.at(card)->getFighter() == FighterType::Sidekick)
                playableCards.push_back(card); 
        }
    return playableCards;
}


vector<int> Game::getPlayableDefenseCard(Character* defender)
{
    vector<int> playableCards;
    bool ishero = defender->isHero();
    auto dhand = otherPlayer->getHero().get()->getDeck().get()->getHand();
    for(int card = 0; card < dhand.size(); card++)
    
        if(dhand.at(card)->getType() == CardType::Defend || 
        dhand.at(card)->getType() == CardType::Versalite)
        {
            if(dhand.at(card)->getFighter() == FighterType::Any)
                playableCards.push_back(card);
            else if(ishero && dhand.at(card)->getFighter() == FighterType::Hero)
                playableCards.push_back(card);
            else if(!ishero && dhand.at(card)->getFighter() == FighterType::Sidekick)
                playableCards.push_back(card);
        }
    return playableCards;
}


vector<int> Game::getSchemeCards(Character* character)
{
    vector<int> playableCards;
    bool ishero = character->isHero();
    auto ahand = currentPlayer->getHero().get()->getDeck().get()->getHand();
    for(int card = 0; card < ahand.size(); card++)
        if(ahand.at(card).get()->getType() == CardType::Scheme)
        {
            if(ishero && ahand.at(card).get()->getFighter() == FighterType::Hero)
                playableCards.push_back(card);
            else if(!ishero && ahand.at(card).get()->getFighter() == FighterType::Sidekick)
                playableCards.push_back(card);
        }
    return playableCards;
}


vector<int> Game::getSidekickPlacement(Character* character)
{
    vector<int> reachable;
    for(int zone : board.getSpace(character->getPosition()).zone)
        for(int i = 0; i < 32; i++)
        {
            if(!canMove(i))
                continue;
            vector<int> zones = board.getSpace(i).zone;
            if(find(zones.begin(), zones.end(), zone) != zones.end())
                reachable.push_back(i);
        }
    return reachable;
}


void Game::changeTurn(){
    swap(currentPlayer, otherPlayer);
    if(canUseAbility && currentPlayer->getHero().get()->getAbility().get()->HasAbilityOnStart())
        currentPlayer->getHero().get()->getAbility().get()->SendRequest(this);
}


void Game::addAction(){
    actionsRemaining++;
}


Player* Game::checkWinner(){
    if(!currentPlayer->getHero()->isAlive())
        return otherPlayer;
    if(!otherPlayer->getHero()->isAlive())
        return currentPlayer;
    return nullptr;
}


vector<AttackOption> Game::getAttackableTargets()
{
    vector<AttackOption> targets;
    for(Character* self : currentPlayer->getAllCharacters()){
        if(self->getPosition() == -1)
            continue;

        for(Character* enemy : otherPlayer->getAllCharacters()){
            if(enemy->getPosition() == -1)
                continue;

            bool canAttack = false;
            if(self->getAttackType() == AttackType::Melee){
                for(int neighbor : board.getSpace(self->getPosition()).neighbors){
                    if(neighbor == enemy->getPosition()){
                        canAttack = true;
                        break;
                    }
                }
            }
            else if(self->getAttackType() == AttackType::Ranged)
            {
                for(int selfZone : board.getSpace(self->getPosition()).zone){
                    for(int enemyZone : board.getSpace(enemy->getPosition()).zone){
                        if(selfZone == enemyZone){
                            canAttack = true;
                            break;
                        }
                    }
                    if(canAttack)
                        break;
                }
                if(!canAttack){
                    for(int neighbor : board.getSpace(self->getPosition()).neighbors){
                        if(neighbor == enemy->getPosition()){
                            canAttack = true;
                            break;
                        }
                    }
                }
            }
            if(canAttack)
                targets.push_back({self, enemy});
        }
    }
    return targets;
}


bool Game::canAttack()
{
    std::vector<AttackOption> targets = getAttackableTargets();
    if(targets.empty()){
        return false;
    }
    for(const auto& com : targets)
        if(!getPlayableAttackCard(com.attacker).empty())
            return true;
    return false;
}


bool Game::canDefense(Character* character){
    if(getPlayableDefenseCard(character).empty())
        return false;
    return true;
}


int Game::boost(Character* self, const int& cardIndex)
{
    int movement = self->getMovement();
    movement += currentPlayer->getHero()->getDeck()->getHand()
    .at(cardIndex)->getBoost();

    currentPlayer->getHero()->getDeck()->discardFromHand(cardIndex);
    return movement;
}


bool Game::canPlayScheme(){
    for(auto c : currentPlayer->getHero()->getDeck()->getHand())
        if(c->getType() == CardType::Scheme)
            return true;
    return false;
}


void Game::playScheme(Character* source, const int& schemeCardIndex)
{
    GameContext context(
        currentPlayer,
        otherPlayer,
        source,
        nullptr,
        &board,
        nullptr,
        nullptr,
        this
    );
    auto schemeCard = currentPlayer->getHero().get()->getDeck()
    ->playCard(schemeCardIndex);

    AttackOption option{
        currentPlayer->getHero().get(), otherPlayer->getHero().get()
    };

    pendingCombat = make_unique<PendingCombat>(
        option, schemeCard, nullptr, context
    );

    continuePlayScheme();
}


void Game::continuePlayScheme(){
    pendingCombat->attackCard.get()->execute(TriggerType::None, pendingCombat->context);

    if(hasPendingAction())
        return;

    currentPlayer->getHero().get()->getDeck()
    .get()->discardCard(pendingCombat->attackCard);

    useAction();
    clearPendingCombat();
}


void Game::combat(AttackOption option, const int& attackCardIndex, 
    std::optional<int> defenseCardIndex){

    shared_ptr<Card> attackCard = currentPlayer->getHero().get()->getDeck()
    .get()->playCard(attackCardIndex);

    shared_ptr<Card> defenseCard = nullptr;

    if(defenseCardIndex.has_value())
        defenseCard = otherPlayer->getHero().get()->getDeck().
        get()->playCard(defenseCardIndex.value());

    GameContext context(
        currentPlayer,
        otherPlayer,
        option.attacker,
        option.target,
        &board,
        attackCard.get(),
        defenseCard.get(),
        this
    );

    pendingCombat = make_unique<PendingCombat>(
        option, attackCard, defenseCard, context
    );

    continueCombat();
}

void Game::continueCombat()
{
    while(pendingCombat)
    {
        switch (pendingCombat.get()->stage)
        {
        /*---------------------------immediat---------------------------*/
        case CombatStage::DefenseImmediate:
        {
            if(pendingCombat->defenseCard)
                pendingCombat.get()->defenseCard->execute(TriggerType::Immediately, 
                    pendingCombat.get()->context);
                    
            if(hasPendingAction())
                return;
            pendingCombat.get()->stage = CombatStage::AttackImmediate;
            continue;
        }
        /*-----------------------------------------------------------------*/
        case CombatStage::AttackImmediate:
        {
            pendingCombat.get()->attackCard->execute(TriggerType::Immediately, 
                pendingCombat.get()->context);
            if(hasPendingAction())
                return;
            pendingCombat.get()->stage = CombatStage::DefenseDuring;
            continue;
        }
        /*---------------------------during---------------------------*/
        case CombatStage::DefenseDuring:
        {
            if(pendingCombat->defenseCard)
                pendingCombat.get()->defenseCard->execute(TriggerType::DuringCombat, 
                    pendingCombat.get()->context);

            if(hasPendingAction())
                return;
            pendingCombat.get()->stage = CombatStage::AttackDuring;
            continue;
        }
        /*-----------------------------------------------------------------*/
        case CombatStage::AttackDuring:
        {
            pendingCombat.get()->attackCard->execute(TriggerType::DuringCombat, 
                pendingCombat.get()->context);
            if(hasPendingAction())
                return;
            pendingCombat.get()->stage = CombatStage::DealDamage;
            continue;
        }
        /*---------------------------damage---------------------------*/
        case CombatStage::DealDamage:
        {
            int damage = calculateDamage(pendingCombat.get()->attackCard.get(), nullptr);
            if(pendingCombat->defenseCard)
                damage = calculateDamage(pendingCombat.get()->attackCard.get(), 
                pendingCombat.get()->defenseCard.get());

            if(damage > 0){
                pendingCombat.get()->option.target->takeDamage(damage);
                pendingCombat.get()->context.setWinner(pendingCombat.get()->option.attacker);
            }
            else {pendingCombat.get()->context.setWinner(pendingCombat.get()->option.target);}
            pendingCombat.get()->stage = CombatStage::DefenseAfter;  
            continue;
        }
        /*---------------------------after---------------------------*/
        case CombatStage::DefenseAfter:
        {
            if(pendingCombat->defenseCard)
                pendingCombat.get()->defenseCard->execute(TriggerType::AfterCombat, 
                    pendingCombat.get()->context);

            if(hasPendingAction())
                return;
            pendingCombat.get()->stage = CombatStage::AttackAfter;
            continue;
        }
        /*-----------------------------------------------------------------*/
        case CombatStage::AttackAfter:
        {
            pendingCombat.get()->attackCard->execute(TriggerType::AfterCombat, 
                pendingCombat.get()->context);
            if(hasPendingAction())
                return;
            pendingCombat.get()->stage = CombatStage::Discard;
            continue;
        }
        /*---------------------------discard---------------------------*/
        case CombatStage::Discard:
        {
            otherPlayer->getHero().get()->getDeck()
                .get()->discardCard(pendingCombat.get()->defenseCard);

            currentPlayer->getHero().get()->getDeck()
                .get()->discardCard(pendingCombat.get()->attackCard);

            pendingCombat.get()->stage = CombatStage::Finished;
            continue;
        }
        /*---------------------------finish---------------------------*/
        case CombatStage::Finished:
        {
            useAction();
            clearPendingCombat();
            return;
        }
        }
    }
}
// ---------------------------------Save Game---------------------------------
const MatchScreenSave& Game::getLoadedMatchScreen() const{
    return loadedMatchScreen;
}
// ---------------------------------------------------------------------------
CharacterRef Game::makeCharacterRef(Character* character) const
{
    if(character == nullptr)
        return {};

    for(int ci = 0; ci < player1.getAllCharacters().size(); ci++){
        if(player1.getAllCharacters().at(ci) == character)
            return {0, ci};
    }
    for(int ci = 0; ci < player2.getAllCharacters().size(); ci++){
        if(player2.getAllCharacters().at(ci) == character)
            return {1, ci};
    }
    return {};
}
// ---------------------------------------------------------------------------
Character* Game::resolveCharacterRef(const CharacterRef& ref)
{
    if(ref.player == 0){
        auto characters = player1.getAllCharacters();
        if(ref.index >= 0 && ref.index < characters.size()){
            return characters[ref.index];
        }
    }
    else if(ref.player == 1){
        auto characters = player2.getAllCharacters();
        if(ref.index >= 0 && ref.index < characters.size()){
            return characters[ref.index];
        }
    }
    return nullptr;
}
// ---------------------------------------------------------------------------
std::vector<PendingSave> Game::createPendingSave() const
{
    std::vector<PendingSave> result;
    for(const auto& ptr : pendingActions)
    {
        if(ptr == nullptr)
            continue;

        PendingAction* action = ptr.get();

        PendingSave save;
        save.type = action->getType();
        switch(action->getType())
        {
        case RequestType::Move:
        {
            auto* move = dynamic_cast<MoveAction*>(action);
            if(move == nullptr)
                break;

            save.currentCharacter = makeCharacterRef(move->getCurrentCharacter());
            save.otherCharacter = makeCharacterRef(move->getOtherCharacter());
            save.mode = static_cast<int>(move->getMode());
            save.range = move->getRange();
            break;
        }
        case RequestType::RaveningST1:
        case RequestType::RaveningST2:
        {
            auto* ravening = dynamic_cast<RaveningAction*>(action);
            if(ravening == nullptr)
                break;

            save.stage = ravening->getStage();
            save.selectedCharacter = makeCharacterRef(ravening->getSelected());
            break;
        }
        case RequestType::CardFromCurrent:
        case RequestType::CardFromOther:
        {
            auto* card = dynamic_cast<ChooseCardAction*>(action);
            if(card == nullptr)
                break;

            if(card->getSelectedPlayer() == &player1)
                save.player = 0;
            else if(card->getSelectedPlayer() == &player2)
                save.player = 1;

            save.minCards = card->getMinCards();
            save.maxCards = card->getMaxCards();
            save.selectedCards = card->getSelectedCards();
            break;
        }
        case RequestType::ShowCard:
        {
            auto* show = dynamic_cast<ShowCardAction*>(action);
            if(show == nullptr)
                break;

            if(show->getSelectedPlayer() == &player1)
                save.player = 0;
            else if(show->getSelectedPlayer() == &player2)
                save.player = 1;
            break;
        }
        case RequestType::Character:
        {
            auto* choose = dynamic_cast<ChooseCharacterAction*>(action);
            if(choose == nullptr)
                break;

            save.mode = static_cast<int>(choose->getMode());
            save.selectedCharacter = makeCharacterRef(choose->getCharacter());
            break;
        }
        case RequestType::DeleteFromCurrent:
        case RequestType::DeleteFromOther:
        {
            auto* deleteAction = dynamic_cast<DeleteCardAction*>(action);
            if(deleteAction == nullptr)
                break;

            if(deleteAction->getSelectedPlayer() == &player1)
                save.player = 0;
            else if(deleteAction->getSelectedPlayer() == &player2)
                save.player = 1;
            break;
        }
        }
        result.push_back(save);
    }

    return result;
}
// ---------------------------------------------------------------------------
std::optional<PendingCombatSave> Game::createPendingCombatSave() const
{
    if(pendingCombat == nullptr)
        return std::nullopt;

    PendingCombatSave save;

    save.attacker = makeCharacterRef(pendingCombat->option.attacker);
    save.target = makeCharacterRef(pendingCombat->option.target);

    if(pendingCombat->attackCard != nullptr)
        save.attackCardId = pendingCombat->attackCard->getId();

    if(pendingCombat->defenseCard != nullptr)
        save.defenseCardId = pendingCombat->defenseCard->getId();

    save.stage = static_cast<int>(pendingCombat->stage);

    save.selection.character = makeCharacterRef(pendingCombat->selection.character);
    save.selection.cards = pendingCombat->selection.cards;
    save.selection.destination = pendingCombat->selection.destination;
    save.selection.showHand = pendingCombat->selection.showHand;
    save.selection.canFinish = pendingCombat->selection.canFinish;
    return save;
}
// ---------------------------------------------------------------------------
GameSave Game::createSaveData(const MatchScreenSave& screenSave) const
{
    GameSave save;

    save.players[0] = player1.createSaveData();
    save.players[1] = player2.createSaveData();

    if(player1 == *currentPlayer)
        save.currentPlayer = 0;
    else
        save.currentPlayer = 1;

    save.canUseAbility = canUseAbility;
    save.remainingAction = actionsRemaining;
    save.pendingActions = createPendingSave();
    if(pendingCombat != nullptr)
        save.pendingCombat = createPendingCombatSave();
    else
        save.pendingCombat.reset();
    save.matchScreen = screenSave;

    return save;
}
// ---------------------------------------------------------------------------
bool Game::SaveGame(const std::string& path, const MatchScreenSave& screenSave) const{
    return SaveManager::saveGame(createSaveData(screenSave), path);
}
// ---------------------------------------------------------------------------
std::shared_ptr<Deck> Game::restoreDeck(const DeckSave& save, HeroType heroType)
{
    std::shared_ptr<Deck> source;
    if(heroType == HeroType::Dracula)
        source = CardFactory::createDraculaDeck();
    else if(heroType == HeroType::Sherlock)
        source = CardFactory::createSherlockDeck();
    else if(heroType == HeroType::Invisibleman)
        source = CardFactory::createInvisibleManDeck();
    else
        return nullptr;

    std::vector<std::shared_ptr<Card>> available;
    for(const auto& card : source->getDrawPile())
        available.push_back(card);

    auto takeCard = [&](const std::string& id) -> std::shared_ptr<Card>
    {
        for(auto it = available.begin(); it != available.end(); ++it)
            if((*it)->getId() == id){
                auto card = *it;
                available.erase(it);
                return card;
            }
        return nullptr;
    };

    std::vector<std::shared_ptr<Card>> draw;
    std::vector<std::shared_ptr<Card>> hand;
    std::vector<std::shared_ptr<Card>> discard;

    for(const auto& id : save.drawPile){
        auto card = takeCard(id);
        if(card != nullptr)
            draw.push_back(card);
    }

    for(const auto& id : save.hand){
        auto card = takeCard(id);
        if(card != nullptr)
            hand.push_back(card);
    }

    for(const auto& id : save.discardPile){
        auto card = takeCard(id);
        if(card != nullptr)
            discard.push_back(card);
    }

    auto result = std::make_shared<Deck>();
    result->restore(draw, hand, discard);
    return result;
}
// ---------------------------------------------------------------------------
std::unique_ptr<PendingAction> Game::restorePendingAction(const PendingSave& save)
{
    switch(save.type)
    {
        case RequestType::Move:
        {
            Character* current = resolveCharacterRef(save.currentCharacter);
            Character* other = resolveCharacterRef(save.otherCharacter);

            if(current == nullptr)
                return nullptr;

            auto action = std::make_unique<MoveAction>(
                current,
                other,
                static_cast<MoveMode>(save.mode),
                save.range
            );
            return action;
        }
        case RequestType::RaveningST1:
        case RequestType::RaveningST2:
        {
            auto action = std::make_unique<RaveningAction>(*this);
            Character* selected = resolveCharacterRef(save.selectedCharacter);

            action->restoreState(selected, save.stage);
            return action;
        }
        case RequestType::CardFromCurrent:
        case RequestType::CardFromOther:
        {
            Player* player = nullptr;
            if(save.player == 0)
                player = &player1;
            else if(save.player == 1)
                player = &player2;
            else
                return nullptr;

            auto action =
                std::make_unique<ChooseCardAction>(
                    player,
                    save.minCards,
                    save.maxCards,
                    *this
                );

            action->restoreState(save.selectedCards);
            return action;
        }
        case RequestType::ShowCard:
        {
            Player* player = nullptr;
            if(save.player == 0)
                player = &player1;
            else if(save.player == 1)
                player = &player2;
            else
                return nullptr;

            return std::make_unique<ShowCardAction>(player);
        }
        case RequestType::Character:
        {
            Character* character = resolveCharacterRef(save.selectedCharacter);

            return std::make_unique<ChooseCharacterAction>(
                static_cast<SelectionMode>(save.mode),
                character
            );
        }
        case RequestType::DeleteFromCurrent:
        case RequestType::DeleteFromOther:
        {
            Player* player = nullptr;
            if(save.player == 0)
                player = &player1;
            else if(save.player == 1)
                player = &player2;
            else
                return nullptr;

            return std::make_unique<DeleteCardAction>(
                *this,
                player
            );
        }
        case RequestType::Dracula:
        {
            return std::make_unique<DraculaAction>();
        }

        default:
            return nullptr;
    }
}
// ---------------------------------------------------------------------------
std::shared_ptr<Card> Game::findCardById(const std::string& id, Character* c) const
{
    if(id.empty())
        return nullptr;

    Hero* hero = nullptr;
    for(Character* mem : player1.getAllCharacters())
        if(c == mem)
            hero = player1.getHero().get();
    if(hero == nullptr)
        hero = player2.getHero().get();

    std::shared_ptr<Deck> source;
    if(hero->getHeroType() == HeroType::Dracula)
        source = CardFactory::createDraculaDeck();
    else if(hero->getHeroType() == HeroType::Sherlock)
        source = CardFactory::createSherlockDeck();
    else
        return nullptr;

    for(const auto& card : source->getDrawPile()){
        if(card != nullptr && card->getId() == id)
            return card;
    }

    return nullptr;
}
// ---------------------------------------------------------------------------
std::unique_ptr<PendingCombat> Game::restorePendingCombat(const PendingCombatSave& save)
{
    Character* attacker = resolveCharacterRef(save.attacker);
    Character* target = resolveCharacterRef(save.target);

    if(attacker == nullptr || target == nullptr)
        return nullptr;

    std::shared_ptr<Card> attackCard = nullptr;
    std::shared_ptr<Card> defenseCard = nullptr;

    if(!save.attackCardId.empty())
    {
        attackCard = findCardById(save.attackCardId, attacker);
        if(attackCard == nullptr)
            return nullptr;
    }

    if(!save.defenseCardId.empty())
    {
        defenseCard = findCardById(save.defenseCardId, target);
        if(defenseCard == nullptr)
            return nullptr;
    }

    AttackOption option;
    option.attacker = attacker;
    option.target = target;

    GameContext context(
        currentPlayer,
        otherPlayer,
        attacker,
        target,
        &board,
        attackCard.get(),
        defenseCard.get(),
        this
    );

    auto combat = std::make_unique<PendingCombat>(
        option,
        attackCard,
        defenseCard,
        context
    );

    combat->stage = static_cast<CombatStage>(save.stage);
    combat->selection.character = resolveCharacterRef(save.selection.character);
    combat->selection.cards = save.selection.cards;
    combat->selection.destination = save.selection.destination;
    combat->selection.showHand = save.selection.showHand;
    combat->selection.canFinish = save.selection.canFinish;
    return combat;
}
// ---------------------------------------------------------------------------
bool Game::LoadGame(const std::string& path)
{
    GameSave save;
    if(!SaveManager::loadGame(save, path))
        return false;

    // ----------------------------------------------------------------
    player1.setName(save.players[0].name);
    player1.setAge(save.players[0].age);

    player2.setName(save.players[1].name);
    player2.setAge(save.players[1].age);
    // ----------------------------------------------------------------
    auto createHero =
        [&](const HeroSave& heroSave) -> std::shared_ptr<Hero>
    {
        if(heroSave.type == HeroType::Dracula){
            this->dracula = HeroFactory::createDracula();
            return dracula;
        }

        if(heroSave.type == HeroType::Sherlock){
            this->sherlock = HeroFactory::createSherlock();
            return sherlock;
        }

        // if(heroSave.type == HeroType::InvisibleMan)
        //     return HeroFactory::createInvisibleMan();

        return nullptr;
    };
    // ----------------------------------------------------------------
    auto restoreHero =
        [](std::shared_ptr<Hero> hero,
           const HeroSave& saveData)
    {
        if(hero == nullptr)
            return;

        hero->setHP(saveData.HP);
        hero->setPosition(saveData.position);

        auto& sidekicks = hero->getSidekicks();
        for(const auto& savedSidekick : saveData.sidekicks)
        {
            if(savedSidekick.index < 0 || savedSidekick.index >= sidekicks.size())
                continue;

            auto& sidekick = sidekicks[savedSidekick.index];
            sidekick->setHP(savedSidekick.HP);
            sidekick->setPosition(savedSidekick.position);
        }
    };
    // ----------------------------------------------------------------
    auto hero1 = createHero(save.players[0].hero);

    if(hero1 == nullptr)
        return false;

    player1.setHero(hero1);
    auto deck1 = restoreDeck(save.players[0].deck, save.players[0].hero.type);

    if(deck1 != nullptr)
        hero1->setDeck(deck1);

    restoreHero(player1.getHero(), save.players[0].hero);
    // ----------------------------------------------------------------
    auto hero2 = createHero(save.players[1].hero);

    if(hero2 == nullptr)
        return false;

    player2.setHero(hero2);

    auto deck2 = restoreDeck(save.players[1].deck, save.players[1].hero.type);
    if(deck2 != nullptr)
        hero2->setDeck(deck2);

    restoreHero(player2.getHero(), save.players[1].hero);
    // ----------------------------------------------------------------
    if(save.currentPlayer == 0)
    {
        currentPlayer = &player1;
        otherPlayer = &player2;
    }
    else if(save.currentPlayer == 1)
    {
        currentPlayer = &player2;
        otherPlayer = &player1;
    }
    else{return false;}
    // ----------------------------------------------------------------
    actionsRemaining = save.remainingAction;
    canUseAbility = save.canUseAbility;

    while(!pendingActions.empty())
        pendingActions.pop_front();

    for(const auto& pendingSave : save.pendingActions){
        auto action = restorePendingAction(pendingSave);
        if(action != nullptr)
            pendingActions.push_back(std::move(action));
    }
    // ----------------------------------------------------------------
    if(save.pendingCombat.has_value()){
        auto combat = restorePendingCombat(*save.pendingCombat);
        if(combat == nullptr)
            return false;

        pendingCombat = std::move(combat);
    }
    else{
        pendingCombat.reset();
    }
    // ----------------------------------------------------------------
    loadedMatchScreen = save.matchScreen;
    return true;
}