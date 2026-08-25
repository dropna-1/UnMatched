#pragma once

#include "IEffects.hpp"
#include "GameContext.hpp"
#include "Game/Cards/Deck.hpp"


class DamageEffect : public IEffect
{
private:
    int damage;

public:
    DamageEffect(int damage);

    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<DamageEffect>(*this);
    }
};


class DrawCardEffect : public IEffect
{
private:
    int count;

public:
    DrawCardEffect(int count);

    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<DrawCardEffect>(*this);
    }
};


class HealEffect : public IEffect
{
private:
    int heal;

public:
    HealEffect(int heal);

    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<HealEffect>(*this);
    }
};


class MoveEffect : public IEffect
{
private:
    int distance;

public:
    MoveEffect(int distance);

    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<MoveEffect>(*this);
    }
};


class DiscardCardEffect : public IEffect
{
private:
    int count;

public:
    DiscardCardEffect(int count);

    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<DiscardCardEffect>(*this);
    }
};


class CancelEffectsEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<CancelEffectsEffect>(*this);
    }
};


class SwapEffect : public IEffect
{
private:
    EffectTarget first;
    EffectTarget second;

public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<SwapEffect>(*this);
    }
};


class MoveToAdjacentEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<MoveToAdjacentEffect>(*this);
    }
};


class DeduceEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<DeduceEffect>(*this);
    }
};


class ReviveSister : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<ReviveSister>(*this);
    }
};


class AmbushEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<AmbushEffect>(*this);
    }
};


class GainActionEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<GainActionEffect>(*this);
    }
};


class FeedingFrenzyEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<FeedingFrenzyEffect>(*this);
    }
};


class RaveningSeduction : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<RaveningSeduction>(*this);
    }
};


class PreyUponEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<PreyUponEffect>(*this);
    }
};


class LookIntoMyEyesEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<LookIntoMyEyesEffect>(*this);
    }
};


class ThirstEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<ThirstEffect>(*this);
    }
};


class RaveningEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<RaveningEffect>(*this);
    }
};


class BeastFormEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<BeastFormEffect>(*this);
    }
};


class ShowHandEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<ShowHandEffect>(*this);
    }
};


class EmergeFromMistEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<EmergeFromMistEffect>(*this);
    }
};


class DreamingOfRevengeEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<DreamingOfRevengeEffect>(*this);
    }
};


class ImpossibleToSeeEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<ImpossibleToSeeEffect>(*this);
    }
};


class RollingFogEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<RollingFogEffect>(*this);
    }
};


class ReignOfTerrorEffect : public IEffect
{
public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<ReignOfTerrorEffect>(*this);
    }
};


class IntoThinAirEffect : public IEffect
{
private:
    int stage = 0;

public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<IntoThinAirEffect>(*this);
    }
};


class SlipAwayEffect : public IEffect
{
private:
    bool waitingForFog = true;

public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<SlipAwayEffect>(*this);
    }
};


class LurkingEffect : public IEffect
{
private:
    int stage = 0;

public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<LurkingEffect>(*this);
    }
};


class StepLightlyEffect : public IEffect
{
private:
    int stage = 0;

public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<StepLightlyEffect>(*this);
    }
};


class CodedNotesEffect : public IEffect
{
private:
    int stage = 0;
    std::vector<int> selectedCards;

public:
    void execute(
        GameContext& context,
        const vector<Character*>& targets
    ) override;

    std::shared_ptr<IEffect> clone() const override
    {
        return std::make_shared<CodedNotesEffect>(*this);
    }
};