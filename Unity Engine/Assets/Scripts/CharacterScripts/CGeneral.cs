using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CGeneral : CharacterBase {
    protected override void Awake() {
        passive = new PassiveEffect(new LinearTarget(new Vector2[]{Vector2.left}, 5, TargetEnum.TEAM), Passive);

        base.Awake();
    }
    private void Passive(Effect pPassive, List<CharacterBase> pTargets, bool pActivate) {
        if (pActivate) {
            foreach (CharacterBase character in pTargets)
                character.damage += 0.3f;
        }
        else {
            foreach (CharacterBase character in pTargets)
                character.damage -= 0.3f;
        }
    }
    public override string GetName() { return "General"; }
    public override string GetDescription() { return "Leader."; }
}
