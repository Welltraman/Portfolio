using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CBarbarian : CharacterBase {
    protected override void Awake() {
        passive = new PassiveEffect(new LinearTarget(new Vector2[]{Vector2.right}, Mathf.Infinity, TargetEnum.ENEMY), Passive);
        //active = new ActiveEffect(3f, 0.8f, new Effect.ApplyAt(Effect.Target.TEAM_BEHIND, 5), ActiveEffect.Timing.ON_EVENT, Active);
        //active.onEventName = "barbarian_guard";
        //attack = new Attack(1.5f, attackSpeed, 1f, new Effect.ApplyAt(Effect.Target.ENEMY_FRONT));

        base.Awake();
    }
    private void Passive(Effect pPassive, List<CharacterBase> pTargets, bool pActivate) {
        if (pActivate) {
            foreach (CharacterBase character in pTargets)
                character.moveSpeed -= 0.2f;
        }
        else {
            foreach (CharacterBase character in pTargets)
                character.moveSpeed += 0.2f;
        }
    }
    private void Active(Effect pActive, bool pActivate) {

    }
    public override void OnCharacterEvent(string pEvent, CharacterBase pCharacter) {
        if (pEvent == "attacked") TriggerEvent("barbarian_guard");
    }
    public override string GetName() { return "Barbarian"; }
    public override string GetDescription() { return "Tough guy."; }
}
