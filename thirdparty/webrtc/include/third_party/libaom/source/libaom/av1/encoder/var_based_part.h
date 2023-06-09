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

#ifndef AOM_AV1_ENCODER_VAR_BASED_PART_H_
#define AOM_AV1_ENCODER_VAR_BASED_PART_H_

#include <stdio.h>

#include "config/aom_config.h"
#include "config/aom_dsp_rtcd.h"
#include "config/av1_rtcd.h"

#include "av1/encoder/encoder.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QINDEX_LOW_THR \
  200  // Use low qindex variance partition thresholds when qindex is below this
       // threshold
#define QINDEX_HIGH_THR \
  220  // Use high qindex variance partition thresholds when qindex is above
       // this threshold

/*!\brief Set the thresholds for variance based partition.
 *
 * Set the variance split thresholds for following the block sizes:
 * 0 - threshold_128x128, 1 - threshold_64x64, 2 - threshold_32x32,
 * 3 - vbp_threshold_16x16. 4 - vbp_threshold_8x8 (to split to 4x4 partition) is
 * currently only used on key frame. The thresholds are based om Q, resolution,
 * noise level, and content state.
 *
 * \ingroup variance_partition
 * \callgraph
 * \callergraph
 *
 * \param[in]       cpi               Top level encoder structure
 * \param[in]       q                 q index
 * \param[in]       content_state     Content state of superblock
 *
 * \return Returns the set of thresholds in \c cpi->vbp_info.thresholds.
 */
void av1_set_variance_partition_thresholds(AV1_COMP *cpi, int q,
                                           int content_state);

/*!\brief Variance based partition selection.
 *
 * Select the partitioning based on the variance of the residual signal,
 * residual generated as the difference between the source and prediction.
 * The prediction is the reconstructed LAST or reconstructed GOLDEN, whichever
 * has lower y sad. For LAST, option exists (speed feature) to use motion
 * compensation based on superblock motion via int_pro_motion_estimation. For
 * key frames reference is fixed 128 level, so variance is the source variance.
 * The variance is computed for downsampled inputs (8x8 or 4x4 downsampled),
 * and selection is done top-down via as set of partition thresholds. defined
 * for each block level, and set based on Q, resolution, noise level, and
 * content state.
 *
 * \ingroup variance_partition
 * \callgraph
 * \callergraph
 *
 * \param[in]       cpi          Top level encoder structure
 * \param[in]       tile         Pointer to TileInfo
 * \param[in]       td           Pointer to ThreadData
 * \param[in]       x            Pointer to MACROBLOCK
 * \param[in]       mi_row       Row coordinate of the superblock in a step
 size of MI_SIZE
 * \param[in]       mi_col       Column coordinate of the super block in a step
 size of MI_SIZE
 *
 * \return Returns the partition in \c xd->mi[0]->sb_type. Also sets the low
 * temporal variance flag and the color sensitivity flag (both used in
 * nonrd_pickmode).
 */
int av1_choose_var_based_partitioning(AV1_COMP *cpi, const TileInfo *const tile,
                                      ThreadData *td, MACROBLOCK *x, int mi_row,
                                      int mi_col);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AOM_AV1_ENCODER_VAR_BASED_PART_H_
