/*
 * Zwifter.hpp
 *
 *  Created on: 5 déc. 2019
 *      Author: vgol
 */

#ifndef SRC_ZWIFTER_HPP_
#define SRC_ZWIFTER_HPP_


#include <stdint.h>
#include <string>
#include <assert.h>
#include <vector>

typedef struct {
	int sec;
	int ftp;
	int rpm;
} sSingleRectangle;


static inline void _check_char_pres(std::string line, const char * const c) {

	if (line.find("@") == std::string::npos) {

		std::cout << "Line is missing char: line=" << std::endl;
		std::cout << line << std::endl;
		assert(0);
	}
}


class Zwifter {
public:
	Zwifter(const char * const name) {

		std::string start = "<workout_file>\n";
		start += "    <author>Vincent Golle</author>\n";
		start += "    <name>";
		start += name;
		start += "</name>\n";
		start += "    <description></description>\n";
		start += "    <sportType>bike</sportType>\n";
		start += "    <tags>\n";
		start += "    </tags>\n";
		start += "    <workout>";

		_series.push_back(start);
	}

	void addSingle(std::string line) {

		// ramp ?
		if (line.find("from") != std::string::npos) {

			// ramp
			parseTriangle(line);
		} else {

			sSingleRectangle descr;
			// normal
			parseRectangle(line, descr);

			char buff[200];

			if (descr.ftp < 1 && descr.rpm) {

				// freeride with cadence
				snprintf(buff, sizeof(buff), "         <FreeRide Duration=\"%d\" Cadence=\"%d\" FlatRoad=\"1\"/>",
						descr.sec,
						descr.rpm);
						
			} else if (descr.ftp < 1) {

				// freeride with no cadence
				snprintf(buff, sizeof(buff), "         <FreeRide Duration=\"%d\" FlatRoad=\"1\"/>",
						descr.sec);
						
			} else if (descr.rpm) {

				snprintf(buff, sizeof(buff), "         <SteadyState Duration=\"%d\" Power=\"%d.%02d\" Cadence=\"%d\"/>",
						descr.sec,
						(int)(descr.ftp / 100), descr.ftp % 100,
						descr.rpm);
			} else {

				snprintf(buff, sizeof(buff), "         <SteadyState Duration=\"%d\" Power=\"%d.%02d\"/>",
						descr.sec,
						(int)(descr.ftp / 100), descr.ftp % 100);
			}

			std::string line = buff;

			_series.push_back(line);
		}
	}

	void addRepetition(std::string line1, std::string line2) {

		// parse nb reps
		int nb_reps = line1.at(0) - '0';
		if (line1.at(1) != 'x') {
			nb_reps = 10 * nb_reps;
			nb_reps += line1.at(1) - '0';
		}

		size_t pos = line1.find(" ");

		sSingleRectangle descr1;
		sSingleRectangle descr2;
		parseRectangle(line1.substr(pos + 1, line1.length()), descr1);
		parseRectangle(line2, descr2);

		char buff[200];
		if (descr1.rpm && descr2.rpm) {

			snprintf(buff, sizeof(buff),
					"         <IntervalsT Repeat=\"%d\" OnDuration=\"%d\" OffDuration=\"%d\" OnPower=\"%d.%02d\" OffPower=\"%d.%02d\" Cadence=\"%d\" CadenceResting=\"%d\"/>",
					nb_reps,
					descr1.sec,
					descr2.sec,
					(int)(descr1.ftp / 100), descr1.ftp % 100,
					(int)(descr2.ftp / 100), descr2.ftp % 100,
					descr1.rpm,
					descr2.rpm);
		} else {

			snprintf(buff, sizeof(buff),
					"         <IntervalsT Repeat=\"%d\" OnDuration=\"%d\" OffDuration=\"%d\" OnPower=\"%d.%02d\" OffPower=\"%d.%02d\"/>",
					nb_reps,
					descr1.sec,
					descr2.sec,
					(int)(descr1.ftp / 100), descr1.ftp % 100,
					(int)(descr2.ftp / 100), descr2.ftp % 100);
		}

		std::string line = buff;

		_series.push_back(line);

	}

	void finish() {

		std::string start = "    </workout>\n";
		start += "</workout_file>\n";

		_series.push_back(start);
	}

	void print() {
		for (auto line : _series)
			std::cout << line << std::endl;
	}

private:
	std::vector<std::string> _series;

	void parseTriangle(std::string line1) {

		std::cout << line1 << std::endl;
		char buff[200];

		if (line1.find("rpm") != std::string::npos) {

			// ramp with cadence
			int sec=0, cad, min=0, ftp1, ftp2;
			int res;
			if (line1.find("min") != std::string::npos && line1.find("sec") != std::string::npos) {

				res = sscanf(line1.c_str(), "%dmin %dsec from %d to %d\% FTP", &min, &sec, &cad, &ftp1, &ftp2);
				assert(res == 5);
			}
			else if (line1.find("min") != std::string::npos) {

				res = sscanf(line1.c_str(), "%dmin @ %drpm, from %d to %d\% FTP", &min, &cad, &ftp1, &ftp2);
				assert(res == 4);

			}
			else {
				res = sscanf(line1.c_str(), "%sec @ %drpm, from %d to %d\% FTP", &sec, &cad, &ftp1, &ftp2);
				assert(res == 4);
			}

			sec += min * 60;

			snprintf(buff, sizeof(buff), "         <Ramp Duration=\"%d\" PowerLow=\"%d.%02d\" PowerHigh=\"%d.%02d\" Cadence=\"%d\"/>",
					sec,
					(int)(ftp1 / 100), ftp1 % 100,
					(int)(ftp2 / 100), ftp2 % 100,
					cad);

		} else {

			// ramp
			int sec=0, min=0, ftp1, ftp2;
			int res;
			if (line1.find("min") != std::string::npos && line1.find("sec") != std::string::npos) {
				res = sscanf(line1.c_str(), "%dmin %dsec from %d to %d\% FTP", &min, &sec, &ftp1, &ftp2);
				assert(res == 4);
			}
			else if (line1.find("min") != std::string::npos) {
				res = sscanf(line1.c_str(), "%dmin from %d to %d\% FTP", &min, &ftp1, &ftp2);
				assert(res == 3);
			}
			else {
				res = sscanf(line1.c_str(), "%dsec from %d to %d\% FTP", &sec, &ftp1, &ftp2);
				assert(res == 3);
			}

			sec += min * 60;

			snprintf(buff, sizeof(buff), "         <Ramp Duration=\"%d\" PowerLow=\"%d.%02d\" PowerHigh=\"%d.%02d\"/>",
					sec,
					(int)(ftp1 / 100), ftp1 % 100,
					(int)(ftp2 / 100), ftp2 % 100);

		}

		std::string line = buff;

		_series.push_back(line);
	}

	void parseTimeRectangle(std::string line, int &sec) {

		char buffer[50];
		std::string sub = "";

		if (line.find("@") != std::string::npos) {
			sub = line.substr(0, line.find('@'));
		} else if (line.find("free") != std::string::npos) {
			sub = line.substr(0, line.find('f'));
		}

		std::cout << sub << std::endl;

		sec = 0;

		if (sub.find("min") != std::string::npos) {

			// min
			int min = 0;
			int res = sscanf(sub.c_str(), "%dmin ", &min);
			assert(res == 1);

			sec = min * 60;

		}

		if (sub.find("sec") != std::string::npos) {

			if (sub.find("min") != std::string::npos) {
				sub = sub.substr(sub.find("min") + 4, sub.length());
				std::cout << sub << std::endl;
			}

			// sec
			int _sec = 0;
			int res = sscanf(sub.c_str(), "%dsec ", &_sec);
			assert(res == 1);

			sec += _sec;
		}
	}

	void parseRectangle(std::string line, sSingleRectangle &descr) {

		int sec, ftp;
		int rpm = 0;

		std::cout << line << std::endl;

		parseTimeRectangle(line, sec);

		if (line.find("free") == std::string::npos) {

			// rpm ?
			if (line.find("rpm") != std::string::npos) {

				_check_char_pres(line, "@");

				std::string sub = line.substr(line.find('@'), line.length());
				std::cout << sub << std::endl;

				int res = sscanf(sub.c_str(), "@ %drpm, %d\% FTP", &rpm, &ftp);
				assert(res == 2);

			} else {

				if (line.find("@") != std::string::npos) {
					line = line.substr(line.find('@')+1, line.length());
				}

				int res = sscanf(line.c_str(), " %d\% FTP", &ftp);
				assert(res == 1);

			}

			descr.sec = sec;
			descr.ftp = ftp;
			descr.rpm = rpm;

		} else {

			// rpm ?
			if (line.find("rpm") != std::string::npos) {

				_check_char_pres(line, "@");

				std::string sub = line.substr(line.find('@'), line.length());
				std::cout << sub << std::endl;

				int res = sscanf(sub.c_str(), "@ %drpm", &rpm);
				assert(res == 1);

			}

			descr.sec = sec;
			descr.ftp = -1;
			descr.rpm = rpm;

		}

	}
};


#endif /* SRC_ZWIFTER_HPP_ */
