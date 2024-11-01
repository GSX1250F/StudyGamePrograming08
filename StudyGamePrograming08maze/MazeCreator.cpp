#include "MazeCreator.h"
#include "Random.h"
#include "Math.h"

int mWidth;
int mHeight;
Vector2 mCell;
Vector2 startCell;
int digCount;
std::vector<Vector2> startCells;
std::vector<std::vector<int> > maze;
const int start = 2;
const int goal = 3;
const int path = 0;
const int wall = 1;

enum Direction { up = 0, right = 1, down = 2, left = 3 };

void Dig(Vector2);
void SetPath(Vector2);
bool GetStartCell();

std::vector<std::vector<int>> MazeCreate(int& width, int& height)
{
    //�����A�c�����Ƃ��Ɋ�ɂ���B
    if (width % 2 == 0) { width++; };
    if (height % 2 == 0) { height++; };
    // ���H����������
    mWidth = width;
    mHeight = height;
    // ���H����z��
    maze.resize(mWidth);
    for (int i = 0; i < mWidth; i++)
    {
        for (int j = 0; j < mHeight; j++)
        {
            maze[i].resize(mHeight);
        }
    }
    // 5�����̃T�C�Y������ł͐����ł��Ȃ�
    if (width >= 5 && height >= 5)
    {
        // �S�Ă�ǂŖ��߂�
        // ���@��J�n���(x,y�Ƃ��ɋ���)���W��ێ����Ă���
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
                {
                    maze[x][y] = path;  // �O�ǂ͔���̈גʘH�ɂ��Ă���(�Ō�ɖ߂�)
                }
                else
                {
                    maze[x][y] = wall;
                }
            }
        }
        //�X�^�[�g�ʒu�������_���ɐݒ�B�ł��邾������ɂ��Ă����B�ŏ���Cell���X�^�[�g�ʒu�Ƃ���B
        Random::Init();
        startCell = Random::GetVector(Vector2::Zero, Vector2(2.9f, 2.9f));
        // 1,3,5�̊�ɂ���
        startCell.x = static_cast<int>(startCell.x) * 2 + 1;
        startCell.y = static_cast<int>(startCell.y) * 2 + 1;
        mCell = startCell;

        // ���@��J�n
        digCount = 0;
        Dig(mCell);

        // �O�ǂ�߂�
        for (int y = 0; y < mHeight; y++)
        {
            for (int x = 0; x < mWidth; x++)
            {
                if (x == 0 || y == 0 || x == mWidth - 1 || y == mHeight - 1)
                {
                    maze[x][y] = wall;
                }
            }
        }
    }
    else {
        //�S���ʘH�̏���Ԃ��i���H�ɂȂ�Ȃ��j�X�^�[�g�͍���A�S�[���͉E��
        for (int i = 0; i < mWidth; i++)
        {
            for (int j = 0; j < mHeight; j++)
            {
                maze[i][j] = path;
            }
        }
        maze[0][0] = start;
        maze[mWidth - 1][mHeight - 1] = goal;
    }
	return maze;
}

// ���W(x, y)�Ɍ����@��
void Dig(Vector2 cell)
{
    // �w����W����@��Ȃ��Ȃ�܂Ŗx�葱����
    while (true)
    {
        // �@��i�߂邱�Ƃ��ł�������̃��X�g���쐬
        std::vector<Direction> directions;
        int x = static_cast<int>(cell.x);
        int y = static_cast<int>(cell.y);
        if (y >= 2) {
            if(maze[x][y - 1] == wall && maze[x][y - 2] == wall) { directions.push_back(up); }
        }
        if (x + 2 <= mWidth - 1) {
            if (maze[x + 1][y] == wall && maze[x + 2][y] == wall) { directions.push_back(right); }
        }
        if (y + 2 <= mHeight - 1) {
            if (maze[x][y + 1] == wall && maze[x][y + 2] == wall) { directions.push_back(down); }
        }
        if (x >= 2) {
            if (maze[x - 1][y] == wall && maze[x - 2][y] == wall) { directions.push_back(left); }
        }
        // �@��i�߂��Ȃ��ꍇ�A���[�v�𔲂���B�@��i�߂��P��ڂȂ炻�����S�[���Ƃ���B
        if (directions.size() == 0) {
            if (digCount == 0)
            {
                maze[x][y] = goal;
            }
            digCount++;

            break;
        }
        else
        {
            // �w����W��ʘH�Ƃ����@������W����폜
            SetPath(cell);
            // �@��i�߂���ꍇ�̓����_���ɕ��������߂Č@��i�߂�
            int dirIndex = Random::GetIntRange(0, directions.size() - 1);
            Random::Init();
            // ���܂��������ɐ�2�}�X����ʘH�Ƃ���
            switch (directions[dirIndex])
            {
            case up:
                cell.y--;
                SetPath(cell);
                cell.y--;
                SetPath(cell);
                break;
            case right:
                cell.x++;
                SetPath(cell);
                cell.x++;
                SetPath(cell);
                break;
            case down:
                cell.y++;
                SetPath(cell);
                cell.y++;
                SetPath(cell);
                break;
            case left:
                cell.x--;
                SetPath(cell);
                cell.x--;
                SetPath(cell);
                break;
            }
        }
        directions.clear();
    }

    // �ǂ��ɂ��@��i�߂��Ȃ��ꍇ�A���@��J�n�����W����@��Ȃ���
    // �����W�����݂��Ȃ��Ƃ��A���@�芮��
    bool flag = GetStartCell();
    if (flag)
    {
        Dig(mCell);
    }
    else
    {
        //start�ʒu��ݒ�
        maze[static_cast<int>(startCell.x)][static_cast<int>(startCell.y)] = start;
    }
}

// ���W��ʘH�Ƃ���(���@��J�n���W���̏ꍇ�͕ێ�)
void SetPath(Vector2 cell)
{
    maze[static_cast<int>(cell.x)][static_cast<int>(cell.y)] = path;
    if ((static_cast<int>(cell.x) % 2 == 1) && (static_cast<int>(cell.y) % 2 == 1))
    {
        // ���@������W
        startCells.push_back(cell);
    }
}

bool GetStartCell()
{
    if (startCells.size() == 0) { return false; }
    else
    {
        // �����_���ɊJ�n���W���擾����
        int index = Random::GetIntRange(0, startCells.size() - 1);
        Random::Init();
        mCell = startCells[index];
        startCells.erase(startCells.begin() + index);
        return true;
    }

}