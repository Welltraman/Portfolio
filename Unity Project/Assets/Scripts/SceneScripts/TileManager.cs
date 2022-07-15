using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class TileManager : MonoBehaviour {
    private Tilemap mPlayerSpawn, mEnemySpawn;
    private Dictionary<Vector3Int, GameObject> mDeployedPlayers = new Dictionary<Vector3Int, GameObject>();
    void Awake() {
        mPlayerSpawn = transform.GetChild(0).GetComponent<Tilemap>();
        mEnemySpawn = transform.GetChild(1).GetComponent<Tilemap>();
    }
    public void SpawnEnemy(List<GameObject> pList) {
        if (pList == null) return;
        List<Vector3Int> availablePos = new List<Vector3Int>();
        foreach(Vector3Int pos in mEnemySpawn.cellBounds.allPositionsWithin) availablePos.Add(pos);
        foreach (GameObject character in pList) {
            int idx = Random.Range(0, availablePos.Count);
            Vector3Int pos = availablePos[idx];
            availablePos.RemoveAt(idx);
            AddEnemy(character, mEnemySpawn.CellToWorld(pos));
        }
    }
    void AddEnemy(GameObject pEnemyRaw, Vector3 pPosition) {
        Vector3 cellSize = mEnemySpawn.cellSize;
        pPosition.x += cellSize.x / 2;
        pPosition.y += cellSize.y / 2;
        GameObject enemy = Instantiate(pEnemyRaw, pPosition, Quaternion.identity);
        enemy.GetComponent<SpriteRenderer>().flipX = true;
        enemy.transform.SetParent(transform.GetChild(1));
    }
    public void AddPlayerCharacter(GameObject pCharacterRaw, Vector3Int pPos) {
        Vector3 pos = pPos;
        Vector3 cellSize = mPlayerSpawn.cellSize;
        pos.x += cellSize.x / 2;
        pos.y += cellSize.y / 2;
        GameObject character = Instantiate(pCharacterRaw, pos, Quaternion.identity);
        character.transform.SetParent(transform.GetChild(0));
        mDeployedPlayers.Add(pPos, character);
    }
    public bool IsPlayerPosNotTaken(Vector3Int pPos) {
        return !mDeployedPlayers.ContainsKey(pPos);
    }
    public GameObject GetPlayerAt(Vector3Int pPos) {
        return mDeployedPlayers[pPos];
    }
    public void RemovePlayerAt(Vector3Int pPos) {
        Destroy(mDeployedPlayers[pPos]);
        mDeployedPlayers.Remove(pPos);
    }
    public void Clear() {
        foreach (Transform child in transform.GetChild(0)) Destroy(child);
        foreach (Transform child in transform.GetChild(1)) Destroy(child);
        mDeployedPlayers.Clear();
    }
    public void ChangeEnemySpawnShade() {
        mEnemySpawn.color = Color.white;
    }
}
