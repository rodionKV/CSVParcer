
#ifndef CSVPARCER_CSVPARSER_H
#define CSVPARCER_CSVPARSER_H

#include <tuple>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


template<class ...Args>
class CSVParser {
public:
    typedef std::tuple<Args...> Tuple_line;
    typedef uint32_t uint;
private:
    std::ifstream &input;
    Tuple_line current_line_tuple;

    uint current_line = -1;
    uint current_column = 0;
    bool file_finish = false;

    char column_delimiter = ',';

public:
    explicit CSVParser(std::ifstream &file) : input(file) {
    }

    virtual void converting(int &value, std::string &s_value, uint column) {
        current_column = column;
        try { value = std::stoi(s_value); }
        catch (std::invalid_argument &invalid_argument) {
            throw std::invalid_argument(invalid_argument.what() +
                                        std::string(" : int = \"") +
                                        s_value + std::string("\" ?"));
        }

    }

    virtual void converting(std::string &value, std::string &s_value, uint column) {
        current_column = column;
        try { value = s_value; }
        catch (std::invalid_argument &invalid_argument) {
            throw std::invalid_argument(invalid_argument.what() +
                                        std::string(" : string = \"") +
                                        s_value + std::string("\" ?"));
        }

    }

    virtual void converting(double &value, std::string &s_value, uint column) {
        current_column = column;
        try { value = std::stod(s_value); }
        catch (std::invalid_argument &invalid_argument) {
            throw std::invalid_argument(invalid_argument.what() +
                                        std::string(" : double = \"") +
                                        s_value + std::string("\" ?"));
        }
    }


    template<class Tuple, std::size_t... Is>
    void for_each_tuple_impl(Tuple &t, std::vector<std::string> &arr,
                             std::index_sequence<Is...>) {
        (converting(std::get<Is>(t), arr[Is], Is), ...);
    }

    void for_each_tuple(std::tuple<Args...> &t, std::vector<std::string> &arr) {
        for_each_tuple_impl(t, arr, std::index_sequence_for<Args...>{});
    }

    void set_tuple_args(std::vector<std::string> column_line_s) {
        for_each_tuple(current_line_tuple, column_line_s);
    }

    virtual Tuple_line next() {
        if (input.is_open() && !input.eof()) {
            current_line++;
            std::vector<std::string> result;
            std::string line;
            std::getline(input, line);

            std::stringstream lineStream(line);
            std::string cell;

            while (std::getline(lineStream, cell, column_delimiter)) {
                result.push_back(cell);
            }

            if (!lineStream && cell.empty()) {

                result.push_back("");

            }
            if (result.size() != sizeof...(Args)) {
                throw std::invalid_argument(
                        "Data mismatch in line(must " + std::to_string(sizeof...(Args)) + " arg): \"" + line + "\"");
            }
            try {
                set_tuple_args(result);
            }
            catch (std::exception &exception) {
                throw std::invalid_argument("Type mismatch in " + std::to_string(current_line) + " line , "
                                            + std::to_string(current_column) + " column\n" + exception.what());
            }

        } else {
            file_finish = true;
        }
        return current_line_tuple;
    }

    virtual bool is_finish() const {
        return file_finish;
    }

    virtual Tuple_line &get_current_line_tuple() {
        return current_line_tuple;
    }

    virtual void restart() {
        if (input.is_open()) {
            input.seekg(0);
            file_finish = false;
            current_column = 0;
            current_line = -1;
        }


    }

    virtual ~CSVParser() {
        if (input.is_open())
            input.close();
    }

};


#endif //CSVPARCER_CSVPARSER_H
