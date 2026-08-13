#include <algorithm>
#include "Game/Characters/SideKick.hpp"
#include "Game/Game.hpp"
#include "Game/Cards/Deck.hpp"
#include "Game/Pending/Pending.hpp"
#include "Game/Factory/CardFactory.hpp"
using namespace std;


Game::Game() : dracula(HeroFactory::createDracula())
, sherlock(HeroFactory::createSherlock()){}


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
    canUseAbility = true;
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
    player.setHero((choice == dracula.get()->getHeroType() ? dracula : sherlock));
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
    pendingActions.push(std::move(action));
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
    pendingActions.pop();
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


Hero* Game::checkWinner(){
    if(!currentPlayer->getHero()->isAlive())
        return otherPlayer->getHero().get();
    if(!otherPlayer->getHero()->isAlive())
        return currentPlayer->getHero().get();
    return nullptr;
}


vector<AttackOption> Game::getAttackableTargets()
{
    vector<AttackOption> targets;
    for(Character* self : currentPlayer->getAllCharacters())
        if(self->getPosition() != -1)
            for(int neighbor : board.getSpace(self->getPosition()).neighbors)
                for(Character* enemy : otherPlayer->getAllCharacters())
                    if(neighbor == enemy->getPosition())
                        targets.push_back({self, enemy});
    return targets;
}


bool Game::canAttack()
{
    if(getAttackableTargets().empty())
        return false;
    for(auto c : currentPlayer->getHero()->getDeck()->getHand())
        if(c->getType() == CardType::Attack || c->getType() == CardType::Versalite)
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