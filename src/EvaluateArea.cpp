/*!
  \file
  \brief ��`�̏d�Ȃ��]��

  \author Satofumi KAMIMURA

  $Id$

  \todo �֐��A����ш����̖��O��K�؂Ȃ��̂ɉ�������
  \todo ������A���Ȃ���

  \todo base �� compare ���\���Ɍ����ꍇ�̃e�X�g�ƕ]����ǉ�����
*/

#include "EvaluateArea.h"
#include "SdlUtils.h"
#include <algorithm>

using namespace beego;


enum {
  X = 0, Y = 1,
  Left = 0, Right = 1,
  Top = 0, Bottom = 1,
};

enum {
  NO_COMMON = 0x00,                // �d�Ȃ��ĂȂ�
  COMMON_SMALL = 0x01,                // �l�̏��������ŏd�Ȃ�
  COMMON_LARGE = 0x02,                // �l�̑傫�����ŏd�Ȃ�
  COMMON_EDGE = COMMON_SMALL | COMMON_LARGE, // �[�ŏd�Ȃ�
  COMMON_OUTER = 0x04,                // ������܂�
  COMMON_INNER = 0x08,                // ����Ɋ܂܂��
  COMMON_SAME = 0x10,                // ����
};

enum {
  ID_1 = 0x001, ID_2 = 0x002, ID_3 = 0x004,
  ID_4 = 0x008, ID_5 = 0x010, ID_6 = 0x020,
  ID_7 = 0x040, ID_8 = 0x080, ID_9 = 0x100,

  ID_ALL = ID_1 | ID_2 | ID_3 | ID_4 | ID_5 | ID_6 | ID_7 | ID_8 | ID_9,
};


static void calculateEdges(int edge[][2], const SDL_Rect& area) {
  edge[X][Left] = area.x;
  edge[X][Right] = area.x + area.w;
  edge[Y][Top] = area.y;
  edge[Y][Bottom] = area.y + area.h;
}


static void judgeAreaOver(int state[2], int base[][2], int compare[][2]) {

  for (int i = 0; i < 2; ++i) {
    if ((base[i][Left] > compare[i][Right]) ||
        (base[i][Right] < compare[i][Left])) {
      // �d�Ȃ��ĂȂ�
      state[i] = NO_COMMON;

    } else if ((base[i][Left] == compare[i][Left]) &&
               (base[i][Right] == compare[i][Right])) {
      // ����
      state[i] = COMMON_SAME;

    } else if ((base[i][Left] <= compare[i][Left]) &&
               (base[i][Right] <= compare[i][Right])) {
      // �����̑傫�����ŏd�Ȃ��Ă���
      state[i] = COMMON_LARGE;

    } else if ((base[i][Left] >= compare[i][Left]) &&
               (base[i][Right] >= compare[i][Right])) {
      // �����̏��������ŏd�Ȃ��Ă���
      state[i] = COMMON_SMALL;

    } else if ((base[i][Left] <= compare[i][Left]) &&
               (base[i][Right] >= compare[i][Right])) {
      // ������܂�
      state[i] = COMMON_OUTER;

    } else if ((base[i][Left] >= compare[i][Left]) &&
               (base[i][Right] <= compare[i][Right])) {
      // ����Ɋ܂܂��
      state[i] = COMMON_INNER;
    }
  }
}


static bool evaluateAreaOver(std::vector<SDL_Rect>& common_area,
                             std::vector<SDL_Rect>& unique_area,
                             const int state[2], int& unique_ids,
                             const SDL_Rect& base_area) {

  // �d�Ȃ肪�Ȃ�
  if ((state[X] == NO_COMMON) || (state[Y] == NO_COMMON)) {
    // common �Ȃ��Aunique �� base �̂܂�
    unique_area.push_back(base_area);
    return true;
  }

  // �̈悪����
  if ((state[X] == COMMON_SAME) && (state[Y] == COMMON_SAME)) {
    // common �� base, unique �Ȃ�
    common_area.push_back(base_area);
    return true;
  }

  // ����Ɋ܂܂��
  if ((state[X] == COMMON_INNER) && (state[Y] == COMMON_INNER)) {
    // common �� base, unique �Ȃ�
    // !!! �̈悪�����ꍇ�Ɠ����Ԃ�l�����ǁA�܂��A�����͕����Ă����܂���
    common_area.push_back(base_area);
    return true;
  }

  // ������܂�
  if ((state[X] == COMMON_OUTER) && (state[Y] == COMMON_OUTER)) {
    // ID_5 �ȊO��ǉ�����
    unique_ids = ID_ALL & ~ID_5;

  } else if ((state[X] & COMMON_EDGE) && (state[Y] & COMMON_EDGE)) {
    // �p���d�Ȃ�
    if (state[Y] == COMMON_LARGE) {
      if (state[X] == COMMON_SMALL) {
        // ��P�ی�
        unique_ids = (ID_2 | ID_3) | ID_6;
      } else {
        // ��Q�ی�
        unique_ids = (ID_1 | ID_2) | ID_4;
      }
    } else {
      if (state[X] != COMMON_SMALL) {
        // ��R�ی�
        unique_ids = ID_4 | (ID_7 | ID_8);
      } else {
        // ��S�ی�
        unique_ids = ID_6 | (ID_8 | ID_9);
      }
    }
  } else if (((state[X] & COMMON_EDGE) ? true : false) ^
             ((state[Y] & COMMON_EDGE) ? true : false)) {
    // �ӂŏd�Ȃ�
    if (state[X] == COMMON_LARGE) {
      // �������͂ݏo��
      if (state[Y] == COMMON_INNER) {
        unique_ids = ID_4;
      } else {
        unique_ids = ID_1 | ID_2 | ID_4 | ID_7 | ID_8;
      }
    } else if (state[X] == COMMON_SMALL) {
      // �E�����͂ݏo��
      if (state[Y] == COMMON_INNER) {
        unique_ids = ID_6;
      } else {
        unique_ids = ID_2 | ID_3 | ID_6 | ID_8 | ID_9;
      }
    } else if (state[Y] == COMMON_LARGE) {
      // �㑤���͂ݏo��
      if (state[X] == COMMON_INNER) {
        unique_ids = ID_2;
      } else {
        unique_ids = ID_1 | ID_2 | ID_3 | ID_4 | ID_6;
      }
    } else if (state[Y] == COMMON_SMALL) {
      // �������͂ݏo��
      if (state[X] == COMMON_INNER) {
        unique_ids = ID_8;
      } else {
        unique_ids = ID_4 | ID_6 | ID_7 | ID_8 | ID_9;
      }
    }
  } else if ((state[X] | state[Y]) == (COMMON_INNER | COMMON_OUTER)) {
    // �\���ŏd�Ȃ�
    if (state[X] == COMMON_INNER) {
      // base ���c���̕�
      unique_ids = ID_2 | ID_8;
    } else {
      // base �������̕�
      unique_ids = ID_4 | ID_6;
    }
  }
  return false;
}


static void calculateAllEdge(std::vector<int> grid[],
                             const int base[][2], const int compare[][2]) {

  for (int i = 0; i < 2; ++i) {
    grid[i].push_back(base[i][Left]);
    grid[i].push_back(base[i][Right]);
    grid[i].push_back(compare[i][Left]);
    grid[i].push_back(compare[i][Right]);

    // �\�[�g����
    sort(grid[i].begin(), grid[i].end());
  }
}


static void createUniqueArea(std::vector<SDL_Rect>& area, int ids,
                             const std::vector<int> edge[2]) {

  while (ids) {
    SDL_Rect rect;

    if ((ids & ID_1) && (ids & ID_2) && (ids & ID_3)) {
      // ��i
      ids &= ~(ID_1 | ID_2 | ID_3);
      set_SdlRect(&rect, edge[X][0], edge[Y][0],
                  edge[X][3] - edge[X][0], edge[Y][1] - edge[Y][0]);

    } else if ((ids & ID_1) && (ids & ID_2)) {
      ids &= ~(ID_1 | ID_2);
      set_SdlRect(&rect, edge[X][0], edge[Y][0],
                  edge[X][2] - edge[X][0], edge[Y][1] - edge[Y][0]);

    } else if ((ids & ID_2) && (ids & ID_3)) {
      ids &= ~(ID_2 | ID_3);
      set_SdlRect(&rect, edge[X][1], edge[Y][0],
                  edge[X][3] - edge[X][1], edge[Y][1] - edge[Y][0]);

    } else if (ids & ID_4) {
      // ���i
      ids &= ~ID_4;
      set_SdlRect(&rect, edge[X][0], edge[Y][1],
                  edge[X][1] - edge[X][0], edge[Y][2] - edge[Y][1]);

    } else if (ids & ID_6) {
      ids &= ~ID_6;
      set_SdlRect(&rect, edge[X][2], edge[Y][1],
                  edge[X][3] - edge[X][2], edge[Y][2] - edge[Y][1]);

    } else if ((ids & ID_7) && (ids & ID_8) && (ids & ID_9)) {
      // ���i
      ids &= ~(ID_7 | ID_8 | ID_9);
      set_SdlRect(&rect, edge[X][0], edge[Y][2],
                  edge[X][3] - edge[X][0], edge[Y][3] - edge[Y][2]);

    } else if ((ids & ID_7) && (ids & ID_8)) {
      ids &= ~(ID_7 | ID_8);
      set_SdlRect(&rect, edge[X][0], edge[Y][2],
                  edge[X][2] - edge[X][0], edge[Y][3] - edge[Y][2]);

    } else if ((ids & ID_8) && (ids & ID_9)) {
      ids &= ~(ID_8 | ID_9);
      set_SdlRect(&rect, edge[X][1], edge[Y][2],
                  edge[X][3] - edge[X][1], edge[Y][3] - edge[Y][2]);

    } else if (0) {
      // �c��̕]��
      // !!!

    } else if (ids & ID_2) {
      // �c��P����
      ids &= ~ID_2;
      set_SdlRect(&rect, edge[X][1], edge[Y][0],
                  edge[X][2] - edge[X][1], edge[Y][1] - edge[Y][0]);

    } else if (ids & ID_8) {
      ids &= ~ID_8;
      set_SdlRect(&rect, edge[X][1], edge[Y][2],
                  edge[X][2] - edge[X][1], edge[Y][3] - edge[Y][2]);
    }

    // �� or ���� ���[���̗̈�͓o�^���Ȃ�
    if ((rect.w != 0) && (rect.h != 0)) {
      area.push_back(rect);
    }
  }
}


void beego::evaluateArea(std::vector<SDL_Rect>& common_area,
                         std::vector<SDL_Rect>& unique_area,
                         const SDL_Rect& base_area,
                         const SDL_Rect& compare_area) {

  if ((base_area.w == 0) || (base_area.h == 0)) {
    // ���肻�̂��̂��A�s��Ȃ�
    return;
  }

  if ((compare_area.w == 0) || (compare_area.h == 0)) {
    // base �̈�� unique �����Ƃ���
    unique_area.push_back(base_area);
    return;
  }

  //fprintf(stderr, "base: %d, %d, %d, %d, compare: %d, %d, %d, %d\n", base_area.x, base_area.y, base_area.w, base_area.h, compare_area.x, compare_area.y, compare_area.w, compare_area.h);

  // �d�Ȃ�̕]��
  int base[2][2];
  int compare[2][2];
  calculateEdges(base, base_area);
  calculateEdges(compare, compare_area);

  // �d�Ȃ蔻��
  int state[2] = { NO_COMMON, NO_COMMON };
  judgeAreaOver(state, base, compare);

  // �d�Ȃ�̏�񂩂�A�̈�ID �̊���U����s��
  int unique_ids = 0;
  if (evaluateAreaOver(common_area, unique_area,
                       state, unique_ids, base_area)) {
    // �֐����ŋ�`�̐����܂ł����������ꍇ�A�����𔲂���
    return;
  }

  // �̈�̍��W�`���v�Z
  std::vector<int> edge[2];
  calculateAllEdge(edge, base, compare);

  // ���ʗ̈�́A��ɐ^�񒆂̃G���A�ɂȂ�
  SDL_Rect area;
  set_SdlRect(&area, edge[X][1], edge[Y][1],
              edge[X][2] - edge[X][1], edge[Y][2] - edge[Y][1]);
  if ((area.w != 0) && (area.h != 0)) {
    common_area.push_back(area);
  }

  // �̈�ID ����A�Ԃ��̈�̐������s��
  createUniqueArea(unique_area, unique_ids, edge);
}


bool beego::isPointInside(int px, int py, const SDL_Rect& area) {

  // �_(px, py) ���A�̈� area ���ɂ���΁Atrue ��Ԃ�
  if (((px >= area.x) && (px <= area.x + area.w)) &&
      ((py >= area.y) && (py <= area.y + area.h))) {
    return true;
  } else {
    return false;
  }
}
