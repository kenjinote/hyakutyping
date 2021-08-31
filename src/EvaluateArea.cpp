/*!
  \file
  \brief 矩形の重なりを評価

  \author Satofumi KAMIMURA

  $Id$

  \todo 関数、および引数の名前を適切なものに改名する
  \todo 実装後、作りなおす

  \todo base と compare が十字に交わる場合のテストと評価を追加する
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
  NO_COMMON = 0x00,                // 重なってない
  COMMON_SMALL = 0x01,                // 値の小さい側で重なる
  COMMON_LARGE = 0x02,                // 値の大きい側で重なる
  COMMON_EDGE = COMMON_SMALL | COMMON_LARGE, // 端で重なる
  COMMON_OUTER = 0x04,                // 相手を含む
  COMMON_INNER = 0x08,                // 相手に含まれる
  COMMON_SAME = 0x10,                // 同じ
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
      // 重なってない
      state[i] = NO_COMMON;

    } else if ((base[i][Left] == compare[i][Left]) &&
               (base[i][Right] == compare[i][Right])) {
      // 同じ
      state[i] = COMMON_SAME;

    } else if ((base[i][Left] <= compare[i][Left]) &&
               (base[i][Right] <= compare[i][Right])) {
      // 自分の大きい側で重なっている
      state[i] = COMMON_LARGE;

    } else if ((base[i][Left] >= compare[i][Left]) &&
               (base[i][Right] >= compare[i][Right])) {
      // 自分の小さい側で重なっている
      state[i] = COMMON_SMALL;

    } else if ((base[i][Left] <= compare[i][Left]) &&
               (base[i][Right] >= compare[i][Right])) {
      // 相手を含む
      state[i] = COMMON_OUTER;

    } else if ((base[i][Left] >= compare[i][Left]) &&
               (base[i][Right] <= compare[i][Right])) {
      // 相手に含まれる
      state[i] = COMMON_INNER;
    }
  }
}


static bool evaluateAreaOver(std::vector<SDL_Rect>& common_area,
                             std::vector<SDL_Rect>& unique_area,
                             const int state[2], int& unique_ids,
                             const SDL_Rect& base_area) {

  // 重なりがない
  if ((state[X] == NO_COMMON) || (state[Y] == NO_COMMON)) {
    // common なし、unique は base のまま
    unique_area.push_back(base_area);
    return true;
  }

  // 領域が同じ
  if ((state[X] == COMMON_SAME) && (state[Y] == COMMON_SAME)) {
    // common が base, unique なし
    common_area.push_back(base_area);
    return true;
  }

  // 相手に含まれる
  if ((state[X] == COMMON_INNER) && (state[Y] == COMMON_INNER)) {
    // common が base, unique なし
    // !!! 領域が同じ場合と同じ返り値だけど、まぁ、実装は分けておきますか
    common_area.push_back(base_area);
    return true;
  }

  // 相手を含む
  if ((state[X] == COMMON_OUTER) && (state[Y] == COMMON_OUTER)) {
    // ID_5 以外を追加する
    unique_ids = ID_ALL & ~ID_5;

  } else if ((state[X] & COMMON_EDGE) && (state[Y] & COMMON_EDGE)) {
    // 角が重なる
    if (state[Y] == COMMON_LARGE) {
      if (state[X] == COMMON_SMALL) {
        // 第１象限
        unique_ids = (ID_2 | ID_3) | ID_6;
      } else {
        // 第２象限
        unique_ids = (ID_1 | ID_2) | ID_4;
      }
    } else {
      if (state[X] != COMMON_SMALL) {
        // 第３象限
        unique_ids = ID_4 | (ID_7 | ID_8);
      } else {
        // 第４象限
        unique_ids = ID_6 | (ID_8 | ID_9);
      }
    }
  } else if (((state[X] & COMMON_EDGE) ? true : false) ^
             ((state[Y] & COMMON_EDGE) ? true : false)) {
    // 辺で重なる
    if (state[X] == COMMON_LARGE) {
      // 左側がはみ出る
      if (state[Y] == COMMON_INNER) {
        unique_ids = ID_4;
      } else {
        unique_ids = ID_1 | ID_2 | ID_4 | ID_7 | ID_8;
      }
    } else if (state[X] == COMMON_SMALL) {
      // 右側がはみ出る
      if (state[Y] == COMMON_INNER) {
        unique_ids = ID_6;
      } else {
        unique_ids = ID_2 | ID_3 | ID_6 | ID_8 | ID_9;
      }
    } else if (state[Y] == COMMON_LARGE) {
      // 上側がはみ出る
      if (state[X] == COMMON_INNER) {
        unique_ids = ID_2;
      } else {
        unique_ids = ID_1 | ID_2 | ID_3 | ID_4 | ID_6;
      }
    } else if (state[Y] == COMMON_SMALL) {
      // 下側がはみ出る
      if (state[X] == COMMON_INNER) {
        unique_ids = ID_8;
      } else {
        unique_ids = ID_4 | ID_6 | ID_7 | ID_8 | ID_9;
      }
    }
  } else if ((state[X] | state[Y]) == (COMMON_INNER | COMMON_OUTER)) {
    // 十字で重なる
    if (state[X] == COMMON_INNER) {
      // base が縦長の方
      unique_ids = ID_2 | ID_8;
    } else {
      // base が横長の方
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

    // ソートする
    sort(grid[i].begin(), grid[i].end());
  }
}


static void createUniqueArea(std::vector<SDL_Rect>& area, int ids,
                             const std::vector<int> edge[2]) {

  while (ids) {
    SDL_Rect rect;

    if ((ids & ID_1) && (ids & ID_2) && (ids & ID_3)) {
      // 上段
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
      // 中段
      ids &= ~ID_4;
      set_SdlRect(&rect, edge[X][0], edge[Y][1],
                  edge[X][1] - edge[X][0], edge[Y][2] - edge[Y][1]);

    } else if (ids & ID_6) {
      ids &= ~ID_6;
      set_SdlRect(&rect, edge[X][2], edge[Y][1],
                  edge[X][3] - edge[X][2], edge[Y][2] - edge[Y][1]);

    } else if ((ids & ID_7) && (ids & ID_8) && (ids & ID_9)) {
      // 下段
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
      // 縦列の評価
      // !!!

    } else if (ids & ID_2) {
      // 残り１つずつ
      ids &= ~ID_2;
      set_SdlRect(&rect, edge[X][1], edge[Y][0],
                  edge[X][2] - edge[X][1], edge[Y][1] - edge[Y][0]);

    } else if (ids & ID_8) {
      ids &= ~ID_8;
      set_SdlRect(&rect, edge[X][1], edge[Y][2],
                  edge[X][2] - edge[X][1], edge[Y][3] - edge[Y][2]);
    }

    // 幅 or 高さ がゼロの領域は登録しない
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
    // 判定そのものを、行わない
    return;
  }

  if ((compare_area.w == 0) || (compare_area.h == 0)) {
    // base 領域を unique 扱いとする
    unique_area.push_back(base_area);
    return;
  }

  //fprintf(stderr, "base: %d, %d, %d, %d, compare: %d, %d, %d, %d\n", base_area.x, base_area.y, base_area.w, base_area.h, compare_area.x, compare_area.y, compare_area.w, compare_area.h);

  // 重なりの評価
  int base[2][2];
  int compare[2][2];
  calculateEdges(base, base_area);
  calculateEdges(compare, compare_area);

  // 重なり判定
  int state[2] = { NO_COMMON, NO_COMMON };
  judgeAreaOver(state, base, compare);

  // 重なりの情報から、領域ID の割り振りを行う
  int unique_ids = 0;
  if (evaluateAreaOver(common_area, unique_area,
                       state, unique_ids, base_area)) {
    // 関数内で矩形の生成までが完了した場合、処理を抜ける
    return;
  }

  // 領域の座標形を計算
  std::vector<int> edge[2];
  calculateAllEdge(edge, base, compare);

  // 共通領域は、常に真ん中のエリアになる
  SDL_Rect area;
  set_SdlRect(&area, edge[X][1], edge[Y][1],
              edge[X][2] - edge[X][1], edge[Y][2] - edge[Y][1]);
  if ((area.w != 0) && (area.h != 0)) {
    common_area.push_back(area);
  }

  // 領域ID から、返す領域の生成を行う
  createUniqueArea(unique_area, unique_ids, edge);
}


bool beego::isPointInside(int px, int py, const SDL_Rect& area) {

  // 点(px, py) が、領域 area 内にあれば、true を返す
  if (((px >= area.x) && (px <= area.x + area.w)) &&
      ((py >= area.y) && (py <= area.y + area.h))) {
    return true;
  } else {
    return false;
  }
}
