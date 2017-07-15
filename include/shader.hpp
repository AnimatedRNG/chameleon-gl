//
// ChameleonGL - A small framework for OpenGL.
// Copyright (C) 2012-2017 Srinivas Kaza
//
// This file is part of ChameleonGL.
//
// ChameleonGL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ChameleonGL is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ChameleonGL.  If not, see <http://www.gnu.org/licenses/>.
//

#pragma once

#include <initializer_list>
#include <sstream>
#include <regex>
#include <vector>
#include <unordered_map>
#include <memory>
#include <assert.h>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "opengl_utils.hpp"
#include "gl_context.hpp"
#include "util.hpp"

#define SO(value) std::shared_ptr<ShaderObject>(new ShaderObject(value))

std::stringstream ss() {
    return std::stringstream();
}

class ShaderExpression {
  public:
    virtual std::string as_string() const = 0;
};

typedef struct {
    std::string type;
    std::string name;
} TypedParameter;

typedef std::shared_ptr<ShaderExpression> ExpressionPtr;
typedef std::vector<ExpressionPtr> ExpressionList;
typedef std::shared_ptr<ExpressionList> ExpressionListPtr;
typedef std::vector<TypedParameter> ShaderParameters;
typedef std::shared_ptr<ShaderParameters> ParametersPtr;

typedef struct {
    std::string return_type;
    std::string name;
    ParametersPtr parameters;
} ShaderFunctionSpec;

inline std::ostream& operator<< (std::ostream& out,
                                 const TypedParameter& param) {
    out << param.type << " " << param.name;

    return out;
}

template <typename T>
inline std::ostream& _vector_stream(std::ostream& out,
                                    const std::vector<T>& params) {
    out << "(";
    for (auto it = params.begin(); it != params.end(); it++) {
        out << *it;
        if (it != params.end() - 1)
            out << ", ";
    }
    out << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out,
                                 const ShaderParameters& params) {
    return _vector_stream(out, params);
}

inline std::ostream& operator<< (std::ostream& out,
                                 const ParametersPtr& params) {
    out << *params;
    return out;
}

inline std::ostream& operator<< (std::ostream& out,
                                 const ShaderFunctionSpec& params) {
    out << params.return_type << " " << params.name << params.parameters;
    return out;
}

inline std::ostream& operator<< (std::ostream& out,
                                 const ShaderExpression* expr) {
    out << expr->as_string();
    return out;
}

inline std::ostream& operator<< (std::ostream& out,
                                 const ExpressionPtr& expr) {
    out << expr->as_string();
    return out;
}

inline std::ostream& operator<< (std::ostream& out,
                                 const ExpressionList& expr_list) {
    _vector_stream(out, expr_list);
    return out;
}

inline std::ostream& operator<< (std::ostream& out,
                                 const ExpressionListPtr& expr_list) {
    out << *expr_list;
    return out;
}

namespace ShaderType {
    const std::string BOOL = "bool";
    const std::string INT = "int";
    const std::string UINT = "uint";
    const std::string FLOAT = "float";
    const std::string DOUBLE = "double";

    const std::string BVEC2 = "bvec2";
    const std::string BVEC3 = "bvec3";
    const std::string BVEC4 = "bvec4";

    const std::string IVEC2 = "ivec2";
    const std::string IVEC3 = "ivec3";
    const std::string IVEC4 = "ivec4";

    const std::string UVEC2 = "uvec2";
    const std::string UVEC3 = "uvec3";
    const std::string UVEC4 = "uvec4";

    const std::string VEC2 = "vec2";
    const std::string VEC3 = "vec3";
    const std::string VEC4 = "vec4";

    const std::string DVEC2 = "dvec2";
    const std::string DVEC3 = "dvec3";
    const std::string DVEC4 = "dvec4";

    const std::string MAT2x2 = "mat2x2";
    const std::string MAT2x3 = "mat2x3";
    const std::string MAT2x4 = "mat2x4";

    const std::string MAT3x2 = "mat3x2";
    const std::string MAT3x3 = "mat3x3";
    const std::string MAT3x4 = "mat3x4";

    const std::string MAT4x2 = "mat4x2";
    const std::string MAT4x3 = "mat4x3";
    const std::string MAT4x4 = "mat4x4";

    const std::string OPAQUE = "opaque";

    const std::string SAMPLER1D = "sampler1d";
    const std::string SAMPLER2D = "sampler2d";
    const std::string SAMPLER3D = "sampler3d";

    const std::string IMAGE = "image";

    const std::string ATOMIC = "atomic";
};

class ShaderObject : public ShaderExpression {
  public:
    ShaderObject(const bool& value) :
        _value(TOS(value)) {
    }

    ShaderObject(const int& value) :
        _value("int(" + TOS(value) + ")") {
    }

    ShaderObject(const uint& value) :
        _value("uint(" + TOS(value) + ")") {
    }

    ShaderObject(const float& value) :
        _value("float(" + TOS(value) + ")") {
    }

    ShaderObject(const double& value) :
        _value("double(" + TOS(value) + ")") {
    }

    ShaderObject(const glm::bvec2& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::bvec3& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::bvec4& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::vec2& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::vec3& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::vec4& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::ivec2& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::ivec3& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::ivec4& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::uvec2& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::uvec3& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::uvec4& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::dvec2& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::dvec3& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::dvec4& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::mat2& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::mat3& value) :
        _value(TOS(value)) { }

    ShaderObject(const glm::mat4& value) :
        _value(TOS(value)) { }

    std::string get_type() const {

    }

    virtual std::string as_string() const override {
        return this->_value;
    }
  private:
    std::string _value;
};

class ShaderFunction : public ShaderExpression {
  public:
    // Note that these dependencies are given in sorted order
    explicit ShaderFunction(const std::string func_name,
                            const std::string source,
                            const std::vector<std::shared_ptr<ShaderFunction>> dependencies) :
        _params(ExpressionListPtr(new ExpressionList)),
        _source(source),
        _spec(),
        _called(false),
        _dependencies(dependencies) {
        std::string cleaned = remove_comments(source);
        std::regex main_regex(names + "\\s+(" + func_name +
                              ")\\s*\\(([^\\)]*)\\)\\s*\\{([^]*)\\}\\s*$");
        std::smatch main_matches;
        std::regex_search(cleaned, main_matches, main_regex);
        assert(main_matches.size() == 5);
        _spec.return_type = main_matches[1];
        _spec.name = main_matches[2];

        ParametersPtr params = ParametersPtr(new ShaderParameters);
        std::string tmp = main_matches[3];
        std::regex parameter_list_regex(names + "\\s+[a-zA-Z0-9]+\\s*");
        std::smatch parameter_matches;
        while (std::regex_search(tmp,
                                 parameter_matches,
                                 parameter_list_regex)) {
            std::string typed_parameter_line = parameter_matches[0];
            std::smatch parameter_expr_matches;
            std::regex parameter_regex(names + "\\s+([a-zA-Z0-9]+)");
            std::regex_search(typed_parameter_line,
                              parameter_expr_matches,
                              parameter_regex);
            TypedParameter param = {
                parameter_expr_matches[1],
                parameter_expr_matches[2]
            };
            params->push_back(param);

            tmp = parameter_matches.suffix();
        }
        _spec.parameters = params;
        _body = main_matches[4];
        std::string error_log = get_errors();
        if (error_log != std::string()) {
            throw std::runtime_error("Error in function " + func_name +
                                     "! Reference: \n\n--------------\n" +
                                     source + "\n--------------\n\n" +
                                     error_log);
        }
    }

    explicit ShaderFunction(const std::string func_name,
                            const std::string source) :
        ShaderFunction(func_name, source,
                       std::vector<std::shared_ptr<ShaderFunction>>()) {
    }

    ShaderFunction operator()(
        std::initializer_list<ExpressionPtr> expression) {
        _params = ExpressionListPtr(new ExpressionList(expression));
        return *this;
    }

    ShaderFunction operator()() {
        return *this;
    }

    template <typename... Rest>
    ShaderFunction operator()(
        ExpressionPtr first = nullptr,
        Rest... rest) {
        _params->push_back(first);
        this->operator()(rest...);
        return *this;
    }

    virtual std::string get_definition() const {
        return TOS(_spec) + " {\n" + TOS(_body) + "\n}";
    }

    virtual std::string get_function_call() const {
        return _spec.name + TOS(_params);
    }

    virtual std::string get_dependencies() const {
        return "";
    }

    virtual std::string as_string() const override {
        return get_function_call();
    }

    void validate() {

    }

  private:
    static std::string combine_names_regex(
        const std::vector<std::string> names) {
        std::string accum = "(";
        for (auto& name : names) {
            accum += name + "|";
        }
        accum = accum.substr(0, accum.length() - 1);
        accum += ")";
        return accum;
    }

    std::string remove_comments(
        const std::string& source) {
        return std::regex_replace(source, std::regex(match_comments), "");
    }

    template <typename A, typename B>
    static std::vector<A> get_keys(const std::unordered_map<A, B> names_map) {
        std::vector<A> vA;
        for (auto const& ap : names_map)
            vA.push_back(ap.first);
        return vA;
    }

    std::string get_errors() {
        std::regex version_regex("#version[^\n\r]+");

        std::regex stage_regex("#define\\s+" + stages);

        std::smatch version_matches, stage_matches;
        std::regex_search(_source, version_matches, version_regex);
        std::regex_search(_source, stage_matches, stage_regex);

        assert(version_matches.size() <= 1);
        assert(stage_matches.size() == 0 || stage_matches.size() == 2);

        GLenum ideal_stage = GL_FRAGMENT_SHADER;
        std::string version_string = "#version 450 core";

        // TODO: Add support for extensions and precision statements

        if (stage_matches.size() == 2) {
            std::string stage_str = stage_matches[1];
            ideal_stage = stages_map.at(stage_str);
        }
        if (version_matches.size() == 1) {
            version_string = version_matches[0];
        }

        std::string defines;
        std::string new_shader = version_string + "\n\n" +
                                 defines + "\n#line 0\n" + get_definition() +
                                 "\n\nvoid main() {}";

        Program program;
        return program.compile_shader(new_shader, ideal_stage, false);
    }

    ExpressionListPtr _params;
    std::string _source;
    ShaderFunctionSpec _spec;
    std::string _body;
    bool _called;
    const std::vector<std::shared_ptr<ShaderFunction>> _dependencies;

    const std::unordered_map<std::string, GLenum> stages_map = {
        {"VERTEX_SHADER_ONLY", GL_VERTEX_SHADER},
        {"FRAGMENT_SHADER_ONLY", GL_FRAGMENT_SHADER},
        {"GEOMETRY_SHADER_ONLY", GL_GEOMETRY_SHADER},
        {"COMPUTE_SHADER_ONLY", GL_COMPUTE_SHADER}
    };
    const std::vector<std::string> stages_declarations = get_keys(stages_map);
    const std::string stages = combine_names_regex(stages_declarations);

    const std::string names = combine_names_regex({
        ShaderType::BOOL,
        ShaderType::INT,
        ShaderType::UINT,
        ShaderType::FLOAT,
        ShaderType::DOUBLE,
        ShaderType::BVEC2,
        ShaderType::BVEC3,
        ShaderType::BVEC4,
        ShaderType::IVEC2,
        ShaderType::IVEC3,
        ShaderType::IVEC4,
        ShaderType::UVEC2,
        ShaderType::UVEC3,
        ShaderType::UVEC4,
        ShaderType::VEC2,
        ShaderType::VEC3,
        ShaderType::VEC4,
        ShaderType::DVEC2,
        ShaderType::DVEC3,
        ShaderType::DVEC4,
        ShaderType::MAT2x2,
        ShaderType::MAT2x3,
        ShaderType::MAT2x4,
        ShaderType::MAT3x2,
        ShaderType::MAT3x3,
        ShaderType::MAT3x4,
        ShaderType::MAT4x2,
        ShaderType::MAT4x3,
        ShaderType::MAT4x4,
        ShaderType::OPAQUE,
        ShaderType::SAMPLER1D,
        ShaderType::SAMPLER2D,
        ShaderType::SAMPLER3D,
        ShaderType::IMAGE,
        ShaderType::ATOMIC,
    });
    const std::string alphanumeric = "[a-zA-Z0-9]";
    const std::string match_all = "*";
    const std::string match_one_or_more = "+";
    const std::string whitespace = "\\s";
    const std::string match_comments =
        "(\\/\\*[\\w\\'\\s\\r\\n\\*]*\\*\\/)"
        "|(\\/\\/[\\w\\s\\']*)|(\\<![\\-\\-\\s\\w\\>\\/]*\\>)";
};

inline std::ostream& operator<< (std::ostream& out,
                                 const ShaderFunction& func) {
    out << func.as_string();
    return out;
}

class Shader {
  public:
  private:
};
