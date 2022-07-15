using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Effects {
    public float damage, health, speed;
    public float[] direction;
}

public abstract class CharacterBase : Observable
{
    public float damage, health, speed;
    private bool mIsMoving = false;
    protected abstract void GetStats();
    protected abstract IEnumerator Attack();
    protected IEnumerator Move() {
        mIsMoving = true;
        yield return new WaitForSeconds(speed);
    }
    void Update() {
        if (mIsMoving) {
            int direction = GetComponent<SpriteRenderer>().flipX ? -1 : 1;
            mIsMoving = false;
        }
    }
    public void TakeDamage(float pDamage) {
        Notify("damageTaken", pDamage);
        health -= pDamage;
        if (health <= 0) {
            Notify("dead", this);
            Destroy(this);
        }
    }
    public Effects Initialize() {
        GetStats();
        return ApplyEffect();
    }
    public abstract Effects ApplyEffect();
    public abstract string GetName();
    public abstract string GetDescription();
}
