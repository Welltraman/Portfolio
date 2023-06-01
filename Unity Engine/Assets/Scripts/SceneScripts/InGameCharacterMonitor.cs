using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameCharacterMonitor : MonoBehaviour {
    private List<GameObject> mPlayerSurvived, mEnemySurvived;
    private Transform mPlayerSide, mEnemySide;
    void Awake() {
        mPlayerSurvived = new List<GameObject>();
        mEnemySurvived = new List<GameObject>();
    }
    public void Init(Transform pPlayerSide, Transform pEnemySide) {
        foreach (Transform child in pPlayerSide) {
            child.GetComponent<CharacterBase>().characterEvent += OnPlayerEvent;
        }
        foreach (Transform child in pEnemySide) {
            child.GetComponent<CharacterBase>().characterEvent += OnEnemyEvent;
        }
        mPlayerSide = pPlayerSide;
        mEnemySide = pEnemySide;
    }
    private void OnPlayerEvent(string pEvent, CharacterBase pCharacter) {
        if (pEvent == "exited") {
            mPlayerSurvived.Add(pCharacter.gameObject);
        }
    }
    private void OnEnemyEvent(string pEvent, CharacterBase pCharacter) {
        if (pEvent == "exited") {
            mEnemySurvived.Add(pCharacter.gameObject);
        }
    }
    // Check for round end ========================================================================
    void Update() {
        if (GameState.currentGameState == GameStateEnum.SIMULATE && DoneSimulation()) {
            GameState.ChangeGameState(GameStateEnum.ROUND_END);
            if (mPlayerSurvived.Count == 0 || mEnemySurvived.Count == 0)
                StartCoroutine(EndGame());
            else StartCoroutine(RestartRound());
        }
    }
    private bool DoneSimulation() {
        foreach (Transform child in mPlayerSide)
            if (!mPlayerSurvived.Contains(child.gameObject)) return false;
        foreach (Transform child in mEnemySide)
            if (!mEnemySurvived.Contains(child.gameObject)) return false;
        return true;
    }
    private IEnumerator RestartRound() {
        yield return null;
        RespawnSurvived();
        GameState.ChangeGameState(GameStateEnum.ROUND_BEGIN);
        Destroy(this);
    }
    private void RespawnSurvived() {
        foreach (GameObject character in mPlayerSurvived) {
            character.SetActive(false);
            CharacterPicker picker = GameManager.GetSceneComponent<CharacterPicker>();
            picker.BringBackImage(character.GetComponent<CharacterBase>().characterDrag);
        }
        GetComponent<TileManager>().DeployEnemyCharacters(mEnemySurvived);
    }
    private IEnumerator EndGame() {
        yield return null;
        GameState.ChangeGameState(GameStateEnum.END);
        Destroy(this);
    }
}
