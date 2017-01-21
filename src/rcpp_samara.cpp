/**
 * @file rcpp_samara.cpp
 * @author See the AUTHORS file
 */

/*
 * Copyright (C) 2012-2017 ULCO http://www.univ-littoral.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Rcpp.h>

#include "rcpp_samara.hpp"

#include <samara/model/kernel/Model.hpp>
#include <samara/model/kernel/Simulator.hpp>
#include <samara/model/models/ModelParameters.hpp>
#include <samara/model/observer/GlobalView.hpp>
#include <samara/model/observer/Observer.hpp>
#include <samara/utils/ParametersReader.hpp>

using namespace Rcpp;

static void format_dates(const model::models::ModelParameters& parameters,
                         std::string& begin, std::string& end)
{
    utils::DateTime::format_date(parameters.get < std::string >("BeginDate"),
                                 begin);
    utils::DateTime::format_date(parameters.get < std::string >("EndDate"),
                                 end);
}

XPtr < Context > rcpp_init_from_database(Rcpp::String name)
{
    Context* context = new Context;
    samara::GlobalParameters globalParameters;
    model::kernel::Model* model = new model::kernel::Model;
    model::models::ModelParameters parameters;
    utils::ParametersReader reader;
    std::string begin;
    std::string end;

    reader.loadFromDatabase(name, parameters);
    format_dates(parameters, begin, end);
    globalParameters.modelVersion = parameters.get < std::string >("IdModele");

    context->begin = utils::DateTime::toJulianDayNumber(begin);
    context->end = utils::DateTime::toJulianDayNumber(end);
    context->simulator = new model::kernel::Simulator(model, globalParameters);

    context->simulator->attachView("global", new model::observer::GlobalView);
    context->simulator->init(context->begin, parameters);
    return XPtr < Context >(context, true);
}

XPtr < Context > rcpp_init_from_json(Rcpp::String json)
{
    Context* context = new Context;
    samara::GlobalParameters globalParameters;
    model::kernel::Model* model = new model::kernel::Model;
    model::models::ModelParameters parameters;
    utils::ParametersReader reader;
    std::string begin;
    std::string end;

    reader.loadFromJSON(json, parameters);
    format_dates(parameters, begin, end);
    globalParameters.modelVersion = parameters.get < std::string >("IdModele");

    context->begin = utils::DateTime::toJulianDayNumber(begin);
    context->end = utils::DateTime::toJulianDayNumber(end);
    context->simulator = new model::kernel::Simulator(model, globalParameters);

    context->simulator->attachView("global", new model::observer::GlobalView);
    context->simulator->init(context->begin, parameters);
    return XPtr < Context >(context, true);
}

// [[Rcpp::export]]
List rcpp_run(SEXP handle)
{
    XPtr < Context > context(handle);

    context->simulator->run(context->begin, context->end);

    const model::observer::Observer& observer = context->simulator->observer();
    const model::observer::Observer::Views& views = observer.views();
    Rcpp::List gresult(views.size());
    Rcpp::CharacterVector gnames;
    unsigned int gindex = 0;

    for (model::observer::Observer::Views::const_iterator it = views.begin();
         it != views.end(); ++it) {
        Rcpp::List result(it->second->values().size() + 1);
        Rcpp::CharacterVector names;
        model::observer::View::Values values = it->second->values();
        double begin = it->second->begin();
        double end = it->second->end();

        gnames.push_back(it->first);
        // write header
        names.push_back("time");
        for (model::observer::View::Values::const_iterator
                 itv = values.begin(); itv != values.end(); ++itv) {
            names.push_back(itv->first);
        }
        // write dates
        {
            Rcpp::CharacterVector values;

            for (double t = begin; t <= end; ++t) {
                values.push_back(utils::DateTime::toJulianDay(t));
            }
            result[0] = values;
        }
        // write values
        unsigned int index = 1;

        for (model::observer::View::Values::const_iterator itv =
                 values.begin(); itv != values.end(); ++itv) {
            model::observer::View::Value::const_iterator itp =
                itv->second.begin();
            Rcpp::NumericVector values;

            for (double t = begin; t <= end; ++t) {
                while (itp != itv->second.end() and itp->first < t) {
                    ++itp;
                }
                if (itp != itv->second.end()) {
                    values.push_back(
                        boost::lexical_cast < double >(itp->second));
                } else {
                    values.push_back(NumericVector::get_na());
                }
            }
            result[index] = values;
            ++index;
        }
        DataFrame out(result);

        out.attr("names") = names;
        gresult[gindex] = out;
        ++gindex;
    }

    gresult.attr("names") = gnames;
    return gresult;
}
