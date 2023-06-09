/*
 * Copyright (c) 2016, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#ifndef AOM_AV1_ENCODER_TOKENIZE_H_
#define AOM_AV1_ENCODER_TOKENIZE_H_

#include "av1/common/entropy.h"
#include "av1/encoder/block.h"
#include "aom_dsp/bitwriter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  aom_cdf_prob *color_map_cdf;
  uint8_t token;
} TokenExtra;

typedef struct {
  TokenExtra *start;
  unsigned int count;
} TokenList;

typedef struct {
  // tile_tok[i][j] is a pointer to the buffer storing palette tokens of the ith
  // tile row, jth tile column.
  TokenExtra *tile_tok[MAX_TILE_ROWS][MAX_TILE_COLS];
  // tplist[i][j][k] holds the start pointer of tile_tok[i][j] and the count of
  // palette tokens for the kth superblock row of the ith tile row, jth tile
  // column.
  TokenList *tplist[MAX_TILE_ROWS][MAX_TILE_COLS];
} TokenInfo;

struct AV1_COMP;
struct ThreadData;
struct FRAME_COUNTS;

enum {
  OUTPUT_ENABLED = 0,
  DRY_RUN_NORMAL,
  DRY_RUN_COSTCOEFFS,
} UENUM1BYTE(RUN_TYPE);

struct tokenize_b_args {
  const struct AV1_COMP *cpi;
  struct ThreadData *td;
  int this_rate;
  uint8_t allow_update_cdf;
  RUN_TYPE dry_run;
};

// Note in all the tokenize functions rate if non NULL is incremented
// with the coefficient token cost only if dry_run = DRY_RUN_COSTCOEFS,
// otherwise rate is not incremented.
void av1_tokenize_sb_vartx(const struct AV1_COMP *cpi, struct ThreadData *td,
                           RUN_TYPE dry_run, BLOCK_SIZE bsize, int *rate,
                           uint8_t allow_update_cdf);

int av1_cost_color_map(const MACROBLOCK *const x, int plane, BLOCK_SIZE bsize,
                       TX_SIZE tx_size, COLOR_MAP_TYPE type);

void av1_tokenize_color_map(const MACROBLOCK *const x, int plane,
                            TokenExtra **t, BLOCK_SIZE bsize, TX_SIZE tx_size,
                            COLOR_MAP_TYPE type, int allow_update_cdf,
                            struct FRAME_COUNTS *counts);

static INLINE int av1_get_tx_eob(const struct segmentation *seg, int segment_id,
                                 TX_SIZE tx_size) {
  const int eob_max = av1_get_max_eob(tx_size);
  return segfeature_active(seg, segment_id, SEG_LVL_SKIP) ? 0 : eob_max;
}

// Token buffer is only used for palette tokens.
static INLINE unsigned int get_token_alloc(int mb_rows, int mb_cols,
                                           int sb_size_log2,
                                           const int num_planes) {
  // Calculate the maximum number of max superblocks in the image.
  const int shift = sb_size_log2 - 4;
  const int sb_size = 1 << sb_size_log2;
  const int sb_size_square = sb_size * sb_size;
  const int sb_rows = ALIGN_POWER_OF_TWO(mb_rows, shift) >> shift;
  const int sb_cols = ALIGN_POWER_OF_TWO(mb_cols, shift) >> shift;

  // One palette token for each pixel. There can be palettes on two planes.
  const int sb_palette_toks = AOMMIN(2, num_planes) * sb_size_square;

  return sb_rows * sb_cols * sb_palette_toks;
}

// Allocate memory for token related info.
static AOM_INLINE void alloc_token_info(AV1_COMMON *cm, TokenInfo *token_info) {
  int mi_rows_aligned_to_sb =
      ALIGN_POWER_OF_TWO(cm->mi_params.mi_rows, cm->seq_params.mib_size_log2);
  int sb_rows = mi_rows_aligned_to_sb >> cm->seq_params.mib_size_log2;
  const int num_planes = av1_num_planes(cm);
  unsigned int tokens =
      get_token_alloc(cm->mi_params.mb_rows, cm->mi_params.mb_cols,
                      MAX_SB_SIZE_LOG2, num_planes);
  CHECK_MEM_ERROR(
      cm, token_info->tile_tok[0][0],
      (TokenExtra *)aom_calloc(tokens, sizeof(*token_info->tile_tok[0][0])));

  CHECK_MEM_ERROR(
      cm, token_info->tplist[0][0],
      (TokenList *)aom_calloc(sb_rows * MAX_TILE_ROWS * MAX_TILE_COLS,
                              sizeof(*token_info->tplist[0][0])));
}

// Free memory from token related variables.
static AOM_INLINE void free_token_info(TokenInfo *token_info) {
  aom_free(token_info->tile_tok[0][0]);
  token_info->tile_tok[0][0] = NULL;

  aom_free(token_info->tplist[0][0]);
  token_info->tplist[0][0] = NULL;
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AOM_AV1_ENCODER_TOKENIZE_H_
