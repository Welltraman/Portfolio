using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CArcher : CharacterBase {
    protected override void Awake() {
        passive = new PassiveEffect(new LinearTarget(new Vector2[]{Vector2.left}, 5, TargetEnum.TEAM), Passive);
        //active = new ActiveEffect(3f, 0.8f, ActiveEffect.Timing.BEFORE_ATTACKED, Active);
        //attack = new Attack(0.2f, attackSpeed, 1f, new Effect.ApplyAt(Effect.Target.ENEMY_RANDOM));

        base.Awake();
    }
    // Passive: speed up the team mates' movement.
    private void Passive(Effect pPassive, List<CharacterBase> pTargets, bool pActivate) {
        if (pActivate) {
            foreach (CharacterBase character in pTargets)
                character.moveSpeed += 0.2f;
        }
        else {
            foreach (CharacterBase character in pTargets)
                character.moveSpeed -= 0.2f;
        }
    }
    // Active: avoid enemy attack by moving to different row.
    private void Active(Effect pActive, bool pActivate) {
        CharacterMover mover = GetComponent<CharacterMover>();
        if (!pActivate || !mover) return;
        mover.SetTargetPos(CharacterMover.Direction.YAXIS);
    }
    public override string GetName() { return "Archer"; }
    public override string GetDescription() { return "Shoots arrows."; }
}
