// ============================================================================
//
// ztd.cuneicode
// Copyright © 2022-2022 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
// Contact: opensource@soasis.org
//
// Commercial License Usage
// Licensees holding valid commercial ztd.cuneicode licenses may use this file
// in accordance with the commercial license agreement provided with the
// Software or, alternatively, in accordance with the terms contained in
// a written agreement between you and Shepherd's Oasis, LLC.
// For licensing terms and conditions see your agreement. For
// further information contact opensource@soasis.org.
//
// Apache License Version 2 Usage
// Alternatively, this file may be used under the terms of Apache License
// Version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// 		http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ========================================================================= //

#include <benchmark/benchmark.h>

#include <ztd/cuneicode.h>
#include <barrier/barrier.h>

#include <vector>

static void utf16_to_utf8_well_formed_cuneicode(benchmark::State& state) {
	const std::vector<char16_t> input_data(c_span_char16_t_data(u16_data),
	     c_span_char16_t_data(u16_data) + c_span_char16_t_size(u16_data));
	std::vector<ztd_char8_t> output_data(c_span_char8_t_size(u8_data));
	bool result = true;
	for (auto _ : state) {
		size_t input_size         = input_data.size();
		const ztd_char16_t* input = input_data.data();
		size_t output_size        = output_data.size();
		ztd_char8_t* output       = output_data.data();
		cnc_mcerror err           = cnc_c16sntoc8sn(&output_size, &output, &input_size, &input);
		if (err != CNC_MCERROR_OKAY) {
			result = false;
		}
	}
	const bool is_equal
	     = std::equal(output_data.cbegin(), output_data.cend(), c_span_char8_t_data(u8_data),
	          c_span_char8_t_data(u8_data) + c_span_char8_t_size(u8_data));
	if (!result || !is_equal) {
		state.SkipWithError("bad benchmark result");
	}
}

static void utf16_to_utf8_well_formed_cuneicode_single(benchmark::State& state) {
	const std::vector<char16_t> input_data(c_span_char16_t_data(u16_data),
	     c_span_char16_t_data(u16_data) + c_span_char16_t_size(u16_data));
	std::vector<ztd_char8_t> output_data(c_span_char8_t_size(u8_data));
	bool result = true;
	for (auto _ : state) {
		size_t input_size         = input_data.size();
		const ztd_char16_t* input = input_data.data();
		size_t output_size        = output_data.size();
		ztd_char8_t* output       = output_data.data();
		for (const ztd_char16_t* const input_last = input + input_size; input != input_last;) {
			cnc_mcerror err = cnc_c16ntoc8n(&output_size, &output, &input_size, &input);
			if (err != CNC_MCERROR_OKAY) {
				result = false;
				break;
			}
		}
	}
	const bool is_equal
	     = std::equal(output_data.cbegin(), output_data.cend(), c_span_char8_t_data(u8_data),
	          c_span_char8_t_data(u8_data) + c_span_char8_t_size(u8_data));
	if (!result || !is_equal) {
		state.SkipWithError("bad benchmark result");
	}
}

static void utf16_to_utf8_well_formed_cuneicode_single_manual_state(benchmark::State& state) {
	const std::vector<char16_t> input_data(c_span_char16_t_data(u16_data),
	     c_span_char16_t_data(u16_data) + c_span_char16_t_size(u16_data));
	std::vector<ztd_char8_t> output_data(c_span_char8_t_size(u8_data));
	bool result = true;
	for (auto _ : state) {
		size_t input_size         = input_data.size();
		const ztd_char16_t* input = input_data.data();
		size_t output_size        = output_data.size();
		ztd_char8_t* output       = output_data.data();
		cnc_mcstate_t state       = {};
		for (const ztd_char16_t* const input_last = input + input_size; input != input_last;) {
			cnc_mcerror err = cnc_c16nrtoc8n(&output_size, &output, &input_size, &input, &state);
			if (err != CNC_MCERROR_OKAY) {
				result = false;
				break;
			}
		}
	}
	const bool is_equal
	     = std::equal(output_data.cbegin(), output_data.cend(), c_span_char8_t_data(u8_data),
	          c_span_char8_t_data(u8_data) + c_span_char8_t_size(u8_data));
	if (!result || !is_equal) {
		state.SkipWithError("bad benchmark result");
	}
}

BENCHMARK(utf16_to_utf8_well_formed_cuneicode);
BENCHMARK(utf16_to_utf8_well_formed_cuneicode_single);
BENCHMARK(utf16_to_utf8_well_formed_cuneicode_single_manual_state);
