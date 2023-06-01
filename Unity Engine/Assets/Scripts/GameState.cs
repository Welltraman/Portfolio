using System;
using UnityEngine;

public enum GameStateEnum {
    MENU,
    ROUND_BEGIN,
    SIMULATE,
    ROUND_END,
    PAUSE,
    RESUME,
    END,
}

public class GameState : MonoBehaviour {
    public delegate void GameStateEvent(GameStateEnum pState);
    public static event GameStateEvent gameStateChanged;
    private static GameStateEnum _currentGameState;
    public static GameStateEnum currentGameState { get => _currentGameState; }
    public static void ChangeGameState(GameStateEnum pState) {
        _currentGameState = pState;
        if (gameStateChanged != null) gameStateChanged(_currentGameState);
    }
    public static void ChangeGameStateString(string pState) {
        _currentGameState = Enum.Parse<GameStateEnum>(pState);
        if (gameStateChanged != null) gameStateChanged(_currentGameState);
    }
}
