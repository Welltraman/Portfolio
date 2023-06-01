using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CHealer : CharacterBase
{
    protected override void Awake() {
        passive = new PassiveEffect(new LinearTarget(new Vector2[]{Vector2.left}, 5, TargetEnum.TEAM), Passive);
        // active = new ActiveEffect();
        // active.cooldown = 1f;
        // active.target = Effect.Target.TEAM_RANDOM;
        // active.health = 0.2f;

        base.Awake();
    }
    private void Passive(Effect pPassive, List<CharacterBase> pTargets, bool pActivate) {
        if (pActivate) {
            foreach (CharacterBase character in pTargets)
                character.health += 1f;
        }
        else {
            foreach (CharacterBase character in pTargets)
                character.health -= 1f;
        }
    }
    public override string GetName() { return "Healer"; }
    public override string GetDescription() { return "Cute girl."; }
}
