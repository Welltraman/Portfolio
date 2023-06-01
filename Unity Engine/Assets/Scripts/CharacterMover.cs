using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterMover : MonoBehaviour {
    public enum Direction {
        FORWARD,
        BACKWARD,
        UP,
        DOWN,
        XAXIS,
        YAXIS,
    }
    private CharacterBase mCharacter;
    private Vector3 mStartPos;
    private Vector3 mTargetPos;
    private float mInterpolate;
    private bool mStopMoving;
    void Awake() {
        GetComponent<CharacterBase>().characterEvent += OnCharacterEvent;
    }
    void Update() {
        if (transform.position != mTargetPos) {
            mInterpolate += Time.deltaTime / (mCharacter.moveSpeed / 2);
            transform.position = Vector3.Lerp(mStartPos, mTargetPos, mInterpolate);
            if (Mathf.Abs(transform.position.x - mTargetPos.x) < 0.01f) EndMoving();
        }
    }
    public IEnumerator Move(CharacterBase pCharacter, Direction pDirection) {
        mCharacter = pCharacter;
        while (!mStopMoving) {
            mCharacter.TriggerEvent("moving");
            mStartPos = transform.position;
            mTargetPos = transform.position;
            SetTargetPos(pDirection);
            yield return new WaitForSeconds(mCharacter.moveSpeed);
        }
    }
    public void SetTargetPos(Direction pDirection) {
        int direction = GetComponent<SpriteRenderer>().flipX ? -1 : 1;
        float cellX = GameManager.GetSceneComponent<TileManager>().playerSpawn.cellSize.x;
        mInterpolate = 0;
        switch (pDirection) {
            case Direction.FORWARD:
                mTargetPos.x = mTargetPos.x + (cellX * direction); break;
            case Direction.BACKWARD:
                mTargetPos.x = mTargetPos.x - (cellX * direction); break;
            case Direction.UP:
                mTargetPos.y = mTargetPos.y - cellX; break;
            case Direction.DOWN:
                mTargetPos.y = mTargetPos.y + cellX; break;
            case Direction.XAXIS:
                int randx = Random.Range(0, 2);
                SetTargetPos(randx == 0 ? Direction.FORWARD : Direction.BACKWARD); break;
            case Direction.YAXIS:
                int randy = Random.Range(0, 2);
                SetTargetPos(randy == 0 ? Direction.UP : Direction.DOWN); break;
        }
    }
    private void EndMoving() {
        transform.position = mTargetPos;
        mInterpolate = 0;
        mCharacter.TriggerEvent("moved");
    }
    private void OnCharacterEvent(string pEvent, CharacterBase pCharacter) {
        if (pEvent == "exited") {
            mStopMoving = true;
            EndMoving();
            GetComponent<CharacterBase>().characterEvent -= OnCharacterEvent;
            Destroy(this);
        }
    }
}
