using UnityEngine;

public class MonoBehaviourPlus : MonoBehaviour {
    protected virtual void Awake() {
        GameState.gameStateChanged += OnGameState;
        GameManager.instance.AddSceneComponent(this);
    }
    protected virtual void OnGameState(GameStateEnum pState) {}
    void OnDestroy() {
        GameState.gameStateChanged -= OnGameState;
        GameManager.instance.RemoveSceneComponent(this);
    }
}
