#include "dxflib++/include/Cadfile.h"
#include "dxflib++/include/entities/entity.h"
#include "dxflib++/include/entities/line.h"
#include "dxflib++/include/entities/lwpolyline.h"
#include "dxflib++/include/entities/text.h"
#include "dxflib++/include/entities/arc.h"
#include "dxflib++/include/entities/spline.h"
#include <fstream>
#include <string>

/**
 * \brief cadfile Constructor
 * \param path Path to the DXF file
 */
dxflib::cadfile::cadfile(const char* path) : filename_(path)
{
	std::ifstream fs;
	fs.open(filename_);
	read_file(fs);
	parse_data();
	linker();
}

dxflib::cadfile::cadfile(const wchar_t* path): wfilename_(path)
{
	std::ifstream fs;
	fs.open(wfilename_);
	read_file(fs);
	parse_data();
	linker();
}


/**
 * \brief Reads the Dxf File
 */
void dxflib::cadfile::read_file(std::ifstream& fs)
{
	if (fs.good())
	{
		// Preallocating memory
		fs.seekg(0, std::istream::end);
		const size_t size{static_cast<size_t>(fs.tellg()) / sizeof(int64_t)};
		fs.seekg(0, std::istream::beg);
		data_.reserve(size);
		// get data
		for (std::string line; std::getline(fs, line);)
		{
			data_.push_back(std::move(line));
		}
		fs.close();
	}
	else
	{
		fs.close();
		throw std::ios::failure("File failed to open/read/close");
	}
}

/**
 * \brief Parses data in the dxf file
 */
void dxflib::cadfile::parse_data()
{
	// Loop Variables
	entities::entity_types current_entity{entities::entity_types::line}; // Current Entity
	bool extraction_flag{false};    // Extraction Flag
	const group_codes::g_common gc; // Common Group Codes - only contains the entity end marker
	const group_codes::start_markers start_markers; // Line Group Codes

	// Buffers
	entities::line_buf lb;           // Line Buffer
	entities::lwpolyline_buffer lwb; // Lwpolyline Buffer
	entities::hatch_buffer hb;       // Hatch Buffer
	entities::text_buffer tb;        // Text Buffer
	entities::arc_buffer ab;         // Arch buffer
	entities::spline_buf splb;       // Spline Buffer
	entities::circle_buffer cb;      // Circle Buffer

	for (int linenum{0}; linenum < static_cast<int>(data_.size()) - 1; ++linenum)
	{
		std::string& cl = data_[linenum];     // The Current line in the data vector
		std::string& nl = data_[linenum + 1]; // The Next line in the data vector
		/*
		 * Assignment Path - First the current entity must be selected from a start marker found
		 * if the DXF file. When the Start marker is found then the extraction flag is set to true
		 * and the current entity is set.
		 */
		if (!extraction_flag)
		{
			if (cl == start_markers.line)
			{
				extraction_flag = true;
				current_entity = entities::entity_types::line;
				continue;
			}
			if (cl == start_markers.lwpolyline)
			{
				extraction_flag = true;
				current_entity = entities::entity_types::lwpolyline;
				continue;
			}
			if (cl == start_markers.hatch)
			{
				extraction_flag = true;
				current_entity = entities::entity_types::hatch;
				continue;
			}
			if (cl == start_markers.text || cl == start_markers.mtext)
			{
				extraction_flag = true;
				current_entity = entities::entity_types::text;
				continue;
			}
			if (cl == start_markers.arc)
			{
				extraction_flag = true;
				current_entity = entities::entity_types::arc;
				continue;
			}
			if (cl == start_markers.spline)
			{
				extraction_flag = true;
				current_entity = entities::entity_types::spline;
				continue;
			}
			if (cl == start_markers.circle)
			{
				extraction_flag = true;
				current_entity = entities::entity_types::circle;
				continue;
			}
		}
		/*
		 * Extraction Path - While the extraction flag is true and the current entity
		 * if know then this function will pass the current line and the next like of the
		 * file to the respective entity parse function.
		 */
		if (extraction_flag)
		{
			switch (current_entity)
			{
			case entities::entity_types::line:
				if (lb.parse(cl, nl))
					linenum++;
				break;
			case entities::entity_types::lwpolyline:
				if (lwb.parse(cl, nl))
					linenum++;
				break;
			case entities::entity_types::hatch:
				if (hb.parse(cl, nl))
					linenum++;
				break;
			case entities::entity_types::text:
				if (tb.parse(cl, nl))
					linenum++;
				break;
			case entities::entity_types::arc:
				if (ab.parse(cl, nl))
					linenum++;
				break;
			case entities::entity_types::spline:
				if (splb.parse(cl, nl))
					linenum++;
				break;
			case entities::entity_types::circle:
				if (cb.parse(cl, nl))
					linenum++;
			default:
				break;
			}
		}
		/*
		 * Build Path - once a end marker is reached the extraction flag is set back to 
		 * false, the entity is constructed and placed into the proper constainer in the cadfile
		 */
		if (cl == gc.end_marker && extraction_flag)
		{
			switch (current_entity)
			{
			case entities::entity_types::line:
				lines_.emplace_back(lb);
				lb.free();
				extraction_flag = false;
				break;
			case entities::entity_types::lwpolyline:
				lwpolylines_.emplace_back(lwb);
				lwb.free();
				extraction_flag = false;
				break;
			case entities::entity_types::hatch:
				hatches_.emplace_back(hb);
				hb.free();
				extraction_flag = false;
				break;
			case entities::entity_types::text:
				basic_text_.emplace_back(tb);
				tb.free();
				extraction_flag = false;
				break;
			case entities::entity_types::arc:
				arcs_.emplace_back(ab);
				ab.free();
				extraction_flag = false;
				break;
			case entities::entity_types::spline:
				splines_.emplace_back(splb);
				splb.free();
				extraction_flag = false;
				break;
			case entities::entity_types::circle:
				circles_.emplace_back(cb);
				cb.free();
				extraction_flag = false;
				break;
			case entities::entity_types::all:
				break;
			}
		}
	}
}


void dxflib::cadfile::linker()
{
	for (auto& hatch : hatches_)
	{
		for (auto& polyline : lwpolylines_)
		{
			if (hatch.get_soft_pointer() == polyline.get_handle())
				hatch.set_lwpolyline(&polyline);
		}
	}
}
