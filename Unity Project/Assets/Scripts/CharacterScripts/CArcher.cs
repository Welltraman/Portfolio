using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CArcher : CharacterBase
{
    protected override void GetStats() {
        damage = 0.2f;
        health = 1.0f;
        speed = 1.0f;
    }
    public override Effects ApplyEffect() { return null; }
    protected override IEnumerator Attack() { return null; }
    public override string GetName() { return "Archer"; }
    public override string GetDescription() { return "Shoots arrows."; }
}
