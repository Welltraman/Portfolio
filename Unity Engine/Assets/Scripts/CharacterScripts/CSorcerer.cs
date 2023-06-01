using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CSorcerer : CharacterBase
{
    protected override void Awake() {
        passive = new PassiveEffect(new LinearTarget(new Vector2[]{Vector2.left}, 5, TargetEnum.TEAM), Passive);

        base.Awake();
    }
    private void Passive(Effect pPassive, List<CharacterBase> pTargets, bool pActivate) {
        if (pActivate) {
            foreach (CharacterBase character in pTargets) {
                character.damage -= 0.5f;
            }
        }
        else {
            foreach (CharacterBase character in pTargets)
                character.damage += 0.5f;
        }
    }
    public override string GetName() { return "Sorcerer"; }
    public override string GetDescription() { return "Shoots magic."; }
}
