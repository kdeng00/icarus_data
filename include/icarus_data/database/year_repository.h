#ifndef YEARREPOSITORY_H_
#define YEARREPOSITORY_H_

#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include <mysql/mysql.h>

#include "icarus_data/database/base_repository.h"


namespace icarus_data { namespace database {
template<class Year, typename Filter, class ConnStr>
class year_repository : public base_repository<ConnStr>
{
public:
    year_repository(const ConnStr &conn_str) : base_repository(conn_str)
    {
    }


    std::vector<Year> retrieveRecords()
    {
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);
        const std::string query = "SELECT * FROM Year";

        mysql_stmt_prepare(stmt, query.c_str(), query.size());
        mysql_stmt_execute(stmt);

        auto yearRecs = parseRecords(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        return yearRecs;
    }

    std::pair<Year, int> retrieveRecordWithSongCount(Year& year, Filter filter)
    {
        std::cout << "retrieving year record with song count\n";
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        qry << "SELECT yr.*, COUNT(*) AS SongCount FROM Year yr LEFT JOIN ";
        qry << "Song sng ON yr.YearId=sng.YearId WHERE ";

        MYSQL_BIND params[1];
        std::memset(params, 0, sizeof(params));

        switch (filter) {
            case Filter::id:
                qry << "sng.YearId = ?";

                params[0].buffer_type = MYSQL_TYPE_LONG;
                params[0].buffer = (char*)&year.id;
                params[0].length = 0;
                params[0].is_null = 0;
                break;
            default:
                break;
        }

        qry << " GROUP BY yr.YearId LIMIT 1";

        const auto query = qry.str();

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        auto yearWSC = parseRecordWithSongCount(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        std::cout << "retrieved year record with song count\n";

        return yearWSC;
    }

    Year retrieveRecord(Year& year, Filter filter)
    {
        std::cout << "retrieving year record\n";
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);
        qry << "SELECT yr.* FROM Year yr WHERE ";

        MYSQL_BIND params[1];
        std::memset(params, 0, sizeof(0));

        switch (filter) {
            case Filter::id:
                qry << "yr.YearId = ?";

                params[0].buffer_type = MYSQL_TYPE_LONG;
                params[0].buffer = reinterpret_cast<char*>(&year.id);
                params[0].length = 0;
                params[0].is_null = 0;
                break;
            case Filter::year:
                qry << "yr.Year = ?";

                params[0].buffer_type = MYSQL_TYPE_LONG;
                params[0].buffer = reinterpret_cast<char*>(&year.song_year);
                params[0].length = 0;
                params[0].is_null = 0;
                break;
            default:
                    break;
        }

        qry << " ORDER BY yr.YearId DESC LIMIT 1";

        const auto query = qry.str();

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        year = parseRecord(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        std::cout << "retrieved record\n";

        return year;
    }


    bool doesYearExist(const Year& year, Filter filter)
    {
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "SELECT * FROM Year WHERE ";

        MYSQL_BIND params[1];
        memset(params, 0, sizeof(params));

        switch (filter) {
            case Filter::id:
                qry << "YearId = ?";

                params[0].buffer_type = MYSQL_TYPE_LONG;
                params[0].buffer = (char*)&year.id;
                params[0].length = 0;
                params[0].is_null = 0;
                break;
            case Filter::year:
                qry << "Year = ?";

                params[0].buffer_type = MYSQL_TYPE_LONG;
                params[0].buffer = (char*)&year.song_year;
                params[0].length = 0;
                params[0].is_null = 0;
                break;
            default:
                break;
        }

        qry << " LIMIT 1";

        const auto query = qry.str();
        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        std::cout << "the query has been performed\n";

        mysql_stmt_store_result(stmt);
        auto rowCount = mysql_stmt_num_rows(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        return (rowCount > 0) ? true : false;
    }


    void saveRecord(const Year& year)
    {
        std::cout << "saving year record\n";

        auto conn = this->setup_connection();
        MYSQL_STMT *stmt = mysql_stmt_init(conn);

        const std::string query("INSERT INTO Year(Year) VALUES(?)");

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());

        MYSQL_BIND params[1];
        memset(params, 0 , sizeof(params));

        params[0].buffer_type = MYSQL_TYPE_LONG;
        params[0].buffer = (char*)&year.song_year;
        params[0].length = 0;
        params[0].is_null = 0;

        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        std::cout << "saved record\n";
    }

    void deleteYear(const Year& year, Filter filter)
    {
        std::cout << "deleting year record\n";
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        qry << "DELETE FROM Year WHERE ";

        MYSQL_BIND params[1];
        std::memset(params, 0, sizeof(params));

        switch (filter) {
            case Filter::id:
                qry << "YearId = ?";

                params[0].buffer_type = MYSQL_TYPE_LONG;
                params[0].buffer = (char*)&year.id;
                params[0].length = 0;
                params[0].is_null = 0;
                break;
            default:
                break;
        }

        const auto query = qry.str();

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        std::cout << "deleted year record\n";
    }

private:
    std::vector<Year> parseRecords(MYSQL_STMT *stmt)
    {
        mysql_stmt_store_result(stmt);
        const auto rowCount = mysql_stmt_num_rows(stmt);

        std::vector<Year> yearRecs;
        yearRecs.reserve(rowCount);

        if (mysql_stmt_field_count(stmt) == 0) 
        {
            std::cout << "field count is 0\n";
            return yearRecs;
        }

        const auto valAmt = 2;
        unsigned long len[valAmt];
        my_bool nullRes[valAmt];

        for (auto status = 0; status == 0; status = mysql_stmt_next_result(stmt))
        {
            if (mysql_stmt_field_count(stmt) > 0)
            {
                Year yearRec;

                std::cout << "fetching statement result\n";
                auto bindedValues = valueBind(yearRec);
                status = mysql_stmt_bind_result(stmt, bindedValues.get());
                status = mysql_stmt_fetch(stmt);

                if (status == 1 || status == MYSQL_NO_DATA) break;

                yearRecs.push_back(yearRec);
            }
            std::cout << "fetching next result\n";
        }

        return yearRecs;
    }

    std::pair<Year, int> parseRecordWithSongCount(MYSQL_STMT *stmt)
    {
        std::cout << "parsing year record\n";
        mysql_stmt_store_result(stmt);

        Year year;
        int songCount = 0;

        if (mysql_stmt_num_rows(stmt) == 0)
        {
            std::cout << "no results\n";
            return std::make_pair(year, songCount);
        }

        auto val = valueBindWithSongCount(year, songCount);

        auto status = mysql_stmt_bind_result(stmt, val.get());
        status = mysql_stmt_fetch(stmt);

        std::cout << "parsed year record from the database\n";

        return std::make_pair(year, songCount);
    }


    std::shared_ptr<MYSQL_BIND> valueBind(Year& year)
    {
        constexpr auto valueCount = 2;
        std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*)
                std::calloc(valueCount, sizeof(MYSQL_BIND)));

        values.get()[0].buffer_type = MYSQL_TYPE_LONG;
        values.get()[0].buffer = reinterpret_cast<char*>(&year.id);

        values.get()[1].buffer_type = MYSQL_TYPE_LONG;
        values.get()[1].buffer = reinterpret_cast<char*>(&year.song_year);

        return values;
    }

    std::shared_ptr<MYSQL_BIND> valueBindWithSongCount(Year& year,
            int& songCount) {
        constexpr auto valueCount = 3;
        std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*)
                std::calloc(valueCount, sizeof(MYSQL_BIND)));

        values.get()[0].buffer_type = MYSQL_TYPE_LONG;
        values.get()[0].buffer = reinterpret_cast<char*>(&year.id);

        values.get()[1].buffer_type = MYSQL_TYPE_LONG;
        values.get()[1].buffer = reinterpret_cast<char*>(&year.song_year);

        values.get()[2].buffer_type = MYSQL_TYPE_LONG;
        values.get()[2].buffer = reinterpret_cast<char*>(&songCount);

        return values;
    }


    Year parseRecord(MYSQL_STMT *stmt) {
        std::cout << "parsing year record\n";
        mysql_stmt_store_result(stmt);

        Year year;
        auto bindedValues = valueBind(year);
        auto status = mysql_stmt_bind_result(stmt, bindedValues.get());
        status = mysql_stmt_fetch(stmt);

        std::cout << "done parsing year record\n";

        return year;
    }
};
}}

#endif
