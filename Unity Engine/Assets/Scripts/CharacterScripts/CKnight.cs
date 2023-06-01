using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CKnight : CharacterBase
{
    protected override void Awake() {
        passive = new PassiveEffect(new LinearTarget(new Vector2[]{Vector2.right}, 3, TargetEnum.ENEMY), Passive);

        // active = new ActiveEffect();
        // active.chance = 0.3f;
        // active.begin = ActiveEffect.Timing.BEFORE_ATTACKED;
        // active.end = ActiveEffect.Timing.AFTER_ATTACKED;
        // active.damage = 0f;

        // attack = new Attack();
        // attack.damage = 1.0f;
        // attack.target = Effect.Target.ENEMY_FRONT;
        // attack.cooldown = speed;

        base.Awake();
    }
    private void Passive(Effect pPassive, List<CharacterBase> pTargets, bool pActivate) {
        if (pActivate) {
            foreach (CharacterBase character in pTargets)
                character.damage -= 0.2f;
        }
        else {
            foreach (CharacterBase character in pTargets)
                character.damage += 0.2f;
        }
    }
    public override string GetName() { return "Knight"; }
    public override string GetDescription() { return "Wears heavy armour."; }
}
