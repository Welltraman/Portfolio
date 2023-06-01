using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public abstract class CharacterBase : MonoBehaviourPlus
{
    [HideInInspector] public GameObject characterDrag; // draggable character image object.
    protected override void Awake() {
        base.Awake();
        AddEffects();
    }
    protected override void OnGameState(GameStateEnum pState) {
        if (pState == GameStateEnum.SIMULATE) StartMoving();
    }
    private void StartMoving() {
        TriggerEvent("starting");
        StartCoroutine(gameObject.AddComponent<CharacterMover>().Move(this, CharacterMover.Direction.FORWARD));
    }
    // Character stats related ====================================================================
    [SerializeField] private float _health;
    public float moveSpeed, attackSpeed, damage;
    public HealthBar healthBar;
    public float health {
        get => _health;
        set {
            _health = value;
            healthBar.OnHealthChange(_health);
        }
    }
    public void TakeDamage(float pDamage) {
        TriggerEvent("attacked");
        health -= pDamage;
        TriggerEvent("damaged");
        if (health <= 0) {
            TriggerEvent("dead");
            Destroy(this.gameObject);
        }
    }
    // Character skills related ===================================================================
    protected PassiveEffect passive;
    protected ActiveEffect active;
    protected Attack attack;
    private void AddEffects() {
        gameObject.AddComponent<EffectManager>().Initialize(passive);
        gameObject.AddComponent<EffectManager>().Initialize(active);
        gameObject.AddComponent<EffectManager>().Initialize(attack);
    }
    // Checking if this character is still inside the map =========================================
    private List<Collider2D> mCollidingBodies = new List<Collider2D>();
    void OnTriggerEnter2D(Collider2D col) { mCollidingBodies.Add(col); }
    void OnTriggerExit2D(Collider2D col) {
        mCollidingBodies.Remove(col);
        if (!mCollidingBodies.Exists(x => x.GetType() == typeof(TilemapCollider2D)))
            TriggerEvent("exited");
    }
    // Character event related ====================================================================
    public delegate void CharacterEvent(string pEvent, CharacterBase pCharacter);
    public event CharacterEvent characterEvent;
    public void TriggerEvent(string pEvent) {
        if (characterEvent != null)
            characterEvent(pEvent, this);
    }
    public virtual void OnCharacterEvent(string pEvent, CharacterBase pSelf) {}
    // Abstract methods ===========================================================================
    public abstract string GetName();
    public abstract string GetDescription();
}
