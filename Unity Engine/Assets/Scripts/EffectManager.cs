using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class Effect {
    public delegate void EffectProcess(Effect pEffect, List<CharacterBase> pTargets, bool pActivate);
    protected EffectTarget mTarget;
    protected EffectProcess mProcess;
}

public class PassiveEffect : Effect {
    public PassiveEffect(EffectTarget pTarget, EffectProcess pProcess) {
        mTarget = pTarget;
        mProcess = pProcess;
    }
}

public class ActiveEffect : Effect {
    public enum Timing {
        NEVER,
        CHANCE,
        COOLDOWN,
        BEFORE_ATTACK,
        AFTER_ATTACK,
        BEFORE_ATTACKED,
        AFTER_ATTACKED,
        BEFORE_MOVE,
        AFTER_MOVE,
        ON_EVENT,
    }
    public string onEventName = "";
    protected float mCooldown = 1f, mChance = 1f;
    protected Timing mBegin = Timing.NEVER; 
    protected Timing mEnd = Timing.NEVER;
    public ActiveEffect() {}
    public ActiveEffect(float pCooldown, float pChance, Timing pTiming, EffectProcess pProcess) {
        mCooldown = pCooldown;
        mChance = pChance;
        mBegin = pTiming;
        mEnd = pTiming;
        mProcess = pProcess;
    }
    public ActiveEffect(float pCooldown, float pChance, EffectTarget pTarget, Timing pTiming, EffectProcess pProcess) {
        mCooldown = pCooldown;
        mChance = pChance;
        mTarget = pTarget;
        mBegin = pTiming;
        mEnd = pTiming;
        mProcess = pProcess;
    }
}

public class Attack : ActiveEffect {
    public Attack(float pCooldown, float pChance, EffectTarget pTarget) {
        mCooldown = pCooldown;
        mChance = pChance;
        mBegin = Timing.AFTER_MOVE;
        mEnd = Timing.BEFORE_MOVE;
        mTarget = pTarget;
    }
}

public enum TargetEnum {
    SELF,
    TEAM,
    ENEMY,
    ALL,
}
public abstract class EffectTarget {
    protected TargetEnum mTarget = TargetEnum.SELF;
    protected CharacterBase mCharacter;
    protected List<CharacterBase> mCharacters = new List<CharacterBase>();
    protected bool mIncludeSelf;
    public virtual void Initialize(CharacterBase pCharacter) => mCharacter = pCharacter;
    public void OnTriggerEnter(Collider2D col) {
        CharacterBase character = col.gameObject.GetComponent<CharacterBase>();
        if (character && !mCharacters.Contains(character) && (character != mCharacter || (character == mCharacter && mIncludeSelf)))
            mCharacters.Add(character);
    }
    public void OnTriggerExit(Collider2D col) {
        CharacterBase character = col.gameObject.GetComponent<CharacterBase>();
        if (character && mCharacters.Contains(character))
            mCharacters.Remove(character);
    }
    public List<CharacterBase> GetTargets() => mCharacters;
}
public class SelfTarget : EffectTarget {
    public override void Initialize(CharacterBase pCharacter) => mCharacters.Add(pCharacter);
}
public class AreaTarget : EffectTarget {
    private int mRange;
    public AreaTarget(int pRange, TargetEnum pTarget, bool pIncludeSelf = false) {
        mRange = pRange;
        mTarget = pTarget;
        mIncludeSelf = pIncludeSelf;
    }
    public override void Initialize(CharacterBase pCharacter) {
        base.Initialize(pCharacter);
        GameObject trigger = new GameObject();
        trigger.transform.SetParent(pCharacter.transform);
        BoxCollider2D col = trigger.AddComponent<BoxCollider2D>();
        col.size = new Vector2(mRange + 1.5f, mRange + 1.5f);
        col.isTrigger = true;
        trigger.AddComponent<AreaTrigger>().Initialize(this);
    }
}
public class RandomTarget : EffectTarget {
    private int mCount;
    public RandomTarget(int pCount, TargetEnum pTarget, bool pIncludeSelf = false) {
        mCount = pCount;
        mTarget = pTarget;
        mIncludeSelf = pIncludeSelf;
    }
    public override void Initialize(CharacterBase pCharacter) {
        for (int i = 0; i < mCount; i++) {
            if (mTarget == TargetEnum.TEAM) mCharacters.Add(GetRandomTeam(pCharacter.transform).GetComponent<CharacterBase>());
            else if (mTarget == TargetEnum.ENEMY) {
                TileManager tile = GameManager.GetSceneComponent<TileManager>();
                Transform enemyParent = pCharacter.transform.parent == tile.playerSpawn.transform ?
                                        tile.enemySpawn.transform : tile.playerSpawn.transform;
                mCharacters.Add(enemyParent.GetChild(Random.Range(0, enemyParent.childCount)).GetComponent<CharacterBase>());
            }
            else if (mTarget == TargetEnum.ALL) {
                TileManager tile = GameManager.GetSceneComponent<TileManager>();
                Transform enemySpawn = pCharacter.transform.parent == tile.playerSpawn.transform ?
                                        tile.enemySpawn.transform : tile.playerSpawn.transform;
                Transform c1 = GetRandomTeam(pCharacter.transform);
                Transform c2 = enemySpawn.GetChild(Random.Range(0, enemySpawn.childCount));
                mCharacters.Add(Random.Range(0, 2) == 0 ? c1.GetComponent<CharacterBase>() : c2.GetComponent<CharacterBase>());
            }
        }
    }
    private Transform GetRandomTeam(Transform pSelf) {
        Transform team;
        while(true) {
            team = pSelf.parent.GetChild(Random.Range(0, pSelf.parent.childCount));
            if (mIncludeSelf || !mIncludeSelf && team != pSelf) break;
        }
        return team;
    }
}
public class LinearTarget : EffectTarget {
    private Vector2[] mDirection;
    private float mDistance;
    public LinearTarget(Vector2[] pDirection, float pDistance, TargetEnum pTarget, bool pIncludeSelf = false) {
        mDirection = pDirection;
        mDistance = pDistance;
        mTarget = pTarget;
        mIncludeSelf = pIncludeSelf;
    }
    public override void Initialize(CharacterBase pCharacter) {
        base.Initialize(pCharacter);
        GameObject trigger = new GameObject();
        trigger.transform.SetParent(pCharacter.transform);
        BoxCollider2D col = trigger.AddComponent<BoxCollider2D>();
        ChangeBoxShape(col);
        col.isTrigger = true;
        trigger.AddComponent<AreaTrigger>().Initialize(this);
    }
    private void ChangeBoxShape(BoxCollider2D col) {
        if (mDistance == Mathf.Infinity) mDistance = 100f;
        Vector2 direction = mDirection[0];
        if (mDirection.Length == 1) {
            col.size = new Vector2(Mathf.Abs(direction.x * mDistance), Mathf.Abs(direction.y * mDistance));
            float offset = mDistance / 2 + 0.5f;
            col.offset = direction * offset;
        }
        else {
            float dist = mDistance * 2 + 1;
            col.size = new Vector2(Mathf.Abs(direction.x * dist), Mathf.Abs(direction.y * dist));
        }
    }
}

public class EffectManager : MonoBehaviour {
    private Effect mEffect;
    void OnEnable() {
        Activate();
    }
    void OnDisable() {
        Deactivate();
    }
    public void Initialize(Effect pEffect) {
        mEffect = pEffect;
        Activate();
    }
    private void Activate() {

    }
    private void Deactivate() {

    }
}
