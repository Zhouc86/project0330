// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_LIBJPEG_TURBO_JPEGLIBMANGLER_H_
#define THIRD_PARTY_LIBJPEG_TURBO_JPEGLIBMANGLER_H_

// Mangle all externally visible function names so we can build our own libjpeg
// without system libraries trying to use it.

#define jpeg_make_c_derived_tbl chromium_jpeg_make_c_derived_tbl
#define jpeg_gen_optimal_table chromium_jpeg_gen_optimal_table
#define jpeg_make_d_derived_tbl chromium_jpeg_make_d_derived_tbl
#define jpeg_fill_bit_buffer chromium_jpeg_fill_bit_buffer
#define jpeg_huff_decode chromium_jpeg_huff_decode
#define jpeg_fdct_islow chromium_jpeg_fdct_islow
#define jpeg_fdct_ifast chromium_jpeg_fdct_ifast
#define jpeg_fdct_float chromium_jpeg_fdct_float
#define jpeg_idct_islow chromium_jpeg_idct_islow
#define jpeg_idct_ifast chromium_jpeg_idct_ifast
#define jpeg_idct_float chromium_jpeg_idct_float
#define jpeg_idct_16x16 chromium_jpeg_idct_16x16
#define jpeg_idct_15x15 chromium_jpeg_idct_15x15
#define jpeg_idct_14x14 chromium_jpeg_idct_14x14
#define jpeg_idct_13x13 chromium_jpeg_idct_13x13
#define jpeg_idct_12x12 chromium_jpeg_idct_12x12
#define jpeg_idct_11x11 chromium_jpeg_idct_11x11
#define jpeg_idct_10x10 chromium_jpeg_idct_10x10
#define jpeg_idct_9x9 chromium_jpeg_idct_9x9
#define jpeg_idct_7x7 chromium_jpeg_idct_7x7
#define jpeg_idct_6x6 chromium_jpeg_idct_6x6
#define jpeg_idct_5x5 chromium_jpeg_idct_5x5
#define jpeg_idct_4x4 chromium_jpeg_idct_4x4
#define jpeg_idct_3x3 chromium_jpeg_idct_3x3
#define jpeg_idct_2x2 chromium_jpeg_idct_2x2
#define jpeg_idct_1x1 chromium_jpeg_idct_1x1
#define jinit_compress_master chromium_jinit_compress_master
#define jinit_c_master_control chromium_jinit_c_master_control
#define jinit_c_main_controller chromium_jinit_c_main_controller
#define jinit_c_prep_controller chromium_jinit_c_prep_controller
#define jinit_c_coef_controller chromium_jinit_c_coef_controller
#define jinit_color_converter chromium_jinit_color_converter
#define jinit_downsampler chromium_jinit_downsampler
#define jinit_forward_dct chromium_jinit_forward_dct
#define jinit_huff_encoder chromium_jinit_huff_encoder
#define jinit_phuff_encoder chromium_jinit_phuff_encoder
#define jinit_marker_writer chromium_jinit_marker_writer
#define jinit_master_decompress chromium_jinit_master_decompress
#define jinit_d_main_controller chromium_jinit_d_main_controller
#define jinit_d_coef_controller chromium_jinit_d_coef_controller
#define jinit_d_post_controller chromium_jinit_d_post_controller
#define jinit_input_controller chromium_jinit_input_controller
#define jinit_marker_reader chromium_jinit_marker_reader
#define jinit_huff_decoder chromium_jinit_huff_decoder
#define jinit_phuff_decoder chromium_jinit_phuff_decoder
#define jinit_inverse_dct chromium_jinit_inverse_dct
#define jinit_upsampler chromium_jinit_upsampler
#define jinit_color_deconverter chromium_jinit_color_deconverter
#define jinit_1pass_quantizer chromium_jinit_1pass_quantizer
#define jinit_2pass_quantizer chromium_jinit_2pass_quantizer
#define jinit_merged_upsampler chromium_jinit_merged_upsampler
#define jinit_memory_mgr chromium_jinit_memory_mgr
#define jdiv_round_up chromium_jdiv_round_up
#define jround_up chromium_jround_up
#define jcopy_sample_rows chromium_jcopy_sample_rows
#define jcopy_block_row chromium_jcopy_block_row
#define jzero_far chromium_jzero_far
#define jpeg_std_error chromium_jpeg_std_error
#define jpeg_CreateCompress chromium_jpeg_CreateCompress
#define jpeg_CreateDecompress chromium_jpeg_CreateDecompress
#define jpeg_destroy_compress chromium_jpeg_destroy_compress
#define jpeg_destroy_decompress chromium_jpeg_destroy_decompress
#define jpeg_stdio_dest chromium_jpeg_stdio_dest
#define jpeg_stdio_src chromium_jpeg_stdio_src
#define jpeg_set_defaults chromium_jpeg_set_defaults
#define jpeg_set_colorspace chromium_jpeg_set_colorspace
#define jpeg_default_colorspace chromium_jpeg_default_colorspace
#define jpeg_set_quality chromium_jpeg_set_quality
#define jpeg_set_linear_quality chromium_jpeg_set_linear_quality
#define jpeg_add_quant_table chromium_jpeg_add_quant_table
#define jpeg_quality_scaling chromium_jpeg_quality_scaling
#define jpeg_simple_progression chromium_jpeg_simple_progression
#define jpeg_suppress_tables chromium_jpeg_suppress_tables
#define jpeg_alloc_quant_table chromium_jpeg_alloc_quant_table
#define jpeg_alloc_huff_table chromium_jpeg_alloc_huff_table
#define jpeg_start_compress chromium_jpeg_start_compress
#define jpeg_write_scanlines chromium_jpeg_write_scanlines
#define jpeg_finish_compress chromium_jpeg_finish_compress
#define jpeg_read_icc_profile chromium_jpeg_read_icc_profile
#define jpeg_write_icc_profile chromium_jpeg_write_icc_profile
#define jpeg_write_raw_data chromium_jpeg_write_raw_data
#define jpeg_write_marker chromium_jpeg_write_marker
#define jpeg_write_m_header chromium_jpeg_write_m_header
#define jpeg_write_m_byte chromium_jpeg_write_m_byte
#define jpeg_write_tables chromium_jpeg_write_tables
#define jpeg_read_header chromium_jpeg_read_header
#define jpeg_start_decompress chromium_jpeg_start_decompress
#define jpeg_read_scanlines chromium_jpeg_read_scanlines
#define jpeg_skip_scanlines chromium_jpeg_skip_scanlines
#define jpeg_crop_scanline chromium_jpeg_crop_scanline
#define jpeg_finish_decompress chromium_jpeg_finish_decompress
#define jpeg_read_raw_data chromium_jpeg_read_raw_data
#define jpeg_has_multiple_scans chromium_jpeg_has_multiple_scans
#define jpeg_start_output chromium_jpeg_start_output
#define jpeg_finish_output chromium_jpeg_finish_output
#define jpeg_input_complete chromium_jpeg_input_complete
#define jpeg_new_colormap chromium_jpeg_new_colormap
#define jpeg_consume_input chromium_jpeg_consume_input
#define jpeg_calc_output_dimensions chromium_jpeg_calc_output_dimensions
#define jpeg_save_markers chromium_jpeg_save_markers
#define jpeg_set_marker_processor chromium_jpeg_set_marker_processor
#define jpeg_read_coefficients chromium_jpeg_read_coefficients
#define jpeg_write_coefficients chromium_jpeg_write_coefficients
#define jpeg_copy_critical_parameters chromium_jpeg_copy_critical_parameters
#define jpeg_abort_compress chromium_jpeg_abort_compress
#define jpeg_abort_decompress chromium_jpeg_abort_decompress
#define jpeg_abort chromium_jpeg_abort
#define jpeg_destroy chromium_jpeg_destroy
#define jpeg_resync_to_restart chromium_jpeg_resync_to_restart
#define jpeg_get_small chromium_jpeg_get_small
#define jpeg_free_small chromium_jpeg_free_small
#define jpeg_get_large chromium_jpeg_get_large
#define jpeg_free_large chromium_jpeg_free_large
#define jpeg_mem_available chromium_jpeg_mem_available
#define jpeg_mem_dest chromium_jpeg_mem_dest
#define jpeg_mem_src chromium_jpeg_mem_src
#define jpeg_open_backing_store chromium_jpeg_open_backing_store
#define jpeg_mem_init chromium_jpeg_mem_init
#define jpeg_mem_term chromium_jpeg_mem_term
#define jpeg_std_message_table chromium_jpeg_std_message_table
#define jpeg_natural_order chromium_jpeg_natural_order

#endif  // THIRD_PARTY_LIBJPEG_TURBO_JPEGLIBMANGLER_H_
