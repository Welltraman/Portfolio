using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class TileManager : MonoBehaviourPlus {
    public Tilemap playerSpawn, enemySpawn;
    [HideInInspector] public Vector3 cellSize;
    protected override void Awake() {
        base.Awake();
        enemySpawn.color = Color.gray;
        cellSize = playerSpawn.cellSize;
        DeployEnemyCharacters(CharacterFactory.GetCharacters(Settings.instance.startingHand));
    }
    protected override void OnGameState(GameStateEnum pState) {
        switch (pState) {
            case GameStateEnum.SIMULATE:
                mDeployedPositions.Clear();
                enemySpawn.color = Color.white;
                gameObject.AddComponent<InGameCharacterMonitor>().Init(playerSpawn.transform, enemySpawn.transform);
                break;
            case GameStateEnum.ROUND_BEGIN:
                enemySpawn.color = Color.gray;
                break;
        }
    }
    public List<Vector2> GetNeighbourPos(Vector2 pOrigin, int pRange) {
        List<Vector2> iPos = new List<Vector2>();
        for (int i = 1; i <= pRange; i++) {
            for (int j = 0; j <= pRange; j++) {
                iPos.Add(new Vector2(pOrigin.x + cellSize.x * i, pOrigin.y + cellSize.y * j));
                iPos.Add(new Vector2(pOrigin.x - cellSize.x * i, pOrigin.y - cellSize.y * j));
                iPos.Add(new Vector2(pOrigin.x + cellSize.x * j, pOrigin.y + cellSize.y * i));
                iPos.Add(new Vector2(pOrigin.x - cellSize.x * j, pOrigin.y - cellSize.y * i));
            }
        }
        List<Vector2> rPos = new List<Vector2>();
        foreach (Vector2 pos in iPos) {
            if (playerSpawn.GetTile(playerSpawn.WorldToCell(pos)) || enemySpawn.GetTile(enemySpawn.WorldToCell(pos)))
                rPos.Add(pos);
        }
        return rPos;
    }
    // Spawning enemy characters ==================================================================
    public void DeployEnemyCharacters(List<GameObject> pEnemies) {
        if (pEnemies == null) return;
        List<Vector3Int> availablePos = GetAvailableEnemySpawn();
        foreach (GameObject enemy in pEnemies) {
            int idx = Random.Range(0, availablePos.Count);
            AddEnemyCharacter(enemy, enemySpawn.CellToWorld(availablePos[idx]));
            availablePos.RemoveAt(idx);
        }
    }
    private List<Vector3Int> GetAvailableEnemySpawn() {
        List<Vector3Int> availablePos = new List<Vector3Int>();
        foreach(Vector3Int pos in enemySpawn.cellBounds.allPositionsWithin)
            availablePos.Add(pos);
        return availablePos;
    }
    private void AddEnemyCharacter(GameObject pEnemyRaw, Vector3 pPosition) {
        pPosition.x += cellSize.x / 2;
        pPosition.y += cellSize.y / 2;
        GameObject enemy = pEnemyRaw;
        if (!pEnemyRaw.scene.IsValid()) { // if this enemy doesn't already exist in scene.
            enemy = Instantiate(pEnemyRaw, pPosition, Quaternion.identity);
            enemy.GetComponent<SpriteRenderer>().flipX = true;
            enemy.transform.SetParent(enemySpawn.transform);
        }
        else enemy.transform.position = pPosition;
    }
    // Spawning player characters =================================================================
    private Dictionary<Vector3Int, GameObject> mDeployedPositions = new Dictionary<Vector3Int, GameObject>();
    public bool DeployPlayerCharacter(Vector3Int pPos, GameObject pCharacterImage) {
        GameObject characterRaw = GetPlayerCharacter(pCharacterImage);
        if (mDeployedPositions.ContainsKey(pPos)) return false;
        GameObject character = AddPlayerCharacter(pPos, characterRaw);
        CharacterBase characterBase = character.GetComponent<CharacterBase>();
        if (!characterBase.characterDrag) characterBase.characterDrag = pCharacterImage;
        mDeployedPositions.Add(pPos, character);
        return true;
    }
    private GameObject GetPlayerCharacter(GameObject pCharacterImage) {
        foreach (Transform child in playerSpawn.transform) {
            if (child.GetComponent<CharacterBase>().characterDrag == pCharacterImage) {
                child.gameObject.SetActive(true);
                return child.gameObject;
            }
        }
        return CharacterFactory.GetCharacter(pCharacterImage.name);
    }
    private GameObject AddPlayerCharacter(Vector3 pPos, GameObject pCharacterRaw) {
        pPos.x += cellSize.x / 2;
        pPos.y += cellSize.y / 2;
        GameObject character = pCharacterRaw;
        if (!pCharacterRaw.scene.IsValid()) {
            character = Instantiate(pCharacterRaw, pPos, Quaternion.identity);
            character.transform.SetParent(playerSpawn.transform);
        }
        else character.transform.position = pPos;
        return character;
    }
    public bool RemoveCharacterAt(Vector3Int pPos, out GameObject oDrag) {
        if (!mDeployedPositions.ContainsKey(pPos)) {
            oDrag = null;
            return false;
        }
        oDrag = mDeployedPositions[pPos].GetComponent<CharacterBase>().characterDrag;
        Destroy(mDeployedPositions[pPos]);
        mDeployedPositions.Remove(pPos);
        return true;
    }
}
