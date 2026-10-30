/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <https://www.gnu.org/licenses/old-licenses/gpl-2.0>.
 */

/** @file fios.cpp Test save/load title sorting. */

#include "../stdafx.h"
#include "../3rdparty/catch2/catch.hpp"
#include "../fios.h"
#include "../string_func.h"

#include "../safeguards.h"

TEST_CASE("Save/load name sorting converts English ordinal dates to ISO dates")
{
	CHECK(FiosMakeDateSortKey("Transport, 24th Oct 1943.sav") == "Transport, 1943-10-24.sav");
	CHECK(FiosMakeDateSortKey("1st Jan 1944") == "1944-01-01");
	CHECK(FiosMakeDateSortKey("2nd Feb 1943") == "1943-02-02");
	CHECK(FiosMakeDateSortKey("3rd Mar 1943") == "1943-03-03");
	CHECK(FiosMakeDateSortKey("11th Apr 1943") == "1943-04-11");
	CHECK(FiosMakeDateSortKey("12th May 1943") == "1943-05-12");
	CHECK(FiosMakeDateSortKey("13th Jun 1943") == "1943-06-13");
	CHECK(FiosMakeDateSortKey("21st Jul 1943") == "1943-07-21");
	CHECK(FiosMakeDateSortKey("22nd Aug 1943") == "1943-08-22");
	CHECK(FiosMakeDateSortKey("23rd Sep 1943") == "1943-09-23");
	CHECK(FiosMakeDateSortKey("30th Nov 1943") == "1943-11-30");
	CHECK(FiosMakeDateSortKey("31st Dec 1943") == "1943-12-31");
	CHECK(FiosMakeDateSortKey("29th feb 2000") == "2000-02-29");
	CHECK(FiosMakeDateSortKey("1st Jan 0000") == "0000-01-01");
	CHECK(FiosMakeDateSortKey("1st Jan 950") == "0950-01-01");
	CHECK(FiosMakeDateSortKey("1st Jan 12345") == "12345-01-01");
	CHECK(FiosMakeDateSortKey("1st Jan 1943 - 24th Oct 1943") == "1943-01-01 - 1943-10-24");
}

TEST_CASE("Save/load name sorting preserves ordinary names and malformed dates")
{
	for (auto title : {"", "ordinary name", "1943-10-24", "0th Oct 1943", "32nd Oct 1943", "11st Oct 1943", "24th Xxx 1943", "24th Oct", "24th Oct 1943extra", "29th Feb 1900", "31st Apr 1943", "124th Oct 1943", "word24th Oct 1943", "24th Oct 99999999999999999999"}) {
		INFO(title);
		CHECK(FiosMakeDateSortKey(title) == title);
	}
}

TEST_CASE("Save/load date keys sort chronologically within the same name prefix")
{
	std::vector<std::string> names = {"Transport, 1st Jan 1944", "Transport, 24th Oct 1943", "Transport, 2nd Oct 1943", "Transport, 31st Dec 1942", "Another, 1st Jan 2000"};
	std::sort(names.begin(), names.end(), [](const auto &a, const auto &b) {
		return StrNaturalCompare(FiosMakeDateSortKey(a), FiosMakeDateSortKey(b)) < 0;
	});
	CHECK(names == std::vector<std::string>{"Another, 1st Jan 2000", "Transport, 31st Dec 1942", "Transport, 2nd Oct 1943", "Transport, 24th Oct 1943", "Transport, 1st Jan 1944"});
}
