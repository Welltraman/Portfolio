using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CBarbarian : CharacterBase
{
    protected override void GetStats() {
        damage = 1.5f;
        health = 2.0f;
        speed = 0.5f;
    }
    public override Effects ApplyEffect() { return null; }
    protected override IEnumerator Attack() { return null; }
    public override string GetName() { return "Barbarian"; }
    public override string GetDescription() { return "Tough guy."; }
}
