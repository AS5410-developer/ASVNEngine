#ifndef INC_BASE_RESULT_OR_ERROR_HPP
#define INC_BASE_RESULT_OR_ERROR_HPP

#include <engine_export.h>
#include <Base/IError.hpp>
#include <string>

namespace AS::Engine {
    template <typename T>
    class ENGINE_EXPORT ResultOrError : public IError {
    public:
        ResultOrError(std::string what);
        ResultOrError(T result);

        T& GetResult();

        virtual const std::string& What() const override;
        virtual bool Failed() const override;
        virtual IError::LogLevel GetLogLevel() const override;

        virtual ~ResultOrError() = default;
    private:
        T Result;
        std::string TError;
        bool IsFailed;
        IError::LogLevel Level;
    };

    template <typename T>
    ResultOrError<T>::ResultOrError(std::string what)
    {
        IsFailed = true;
        TError = what;
    }
    template <typename T>
    ResultOrError<T>::ResultOrError(T result)
    {
        IsFailed = false;
        Result = result;
    }
    template <typename T>
    T& ResultOrError<T>::GetResult(){
        return Result;
    }
    template <typename T>
    const std::string& ResultOrError<T>::What() const
    {
        return TError;
    }
    template <typename T>
    bool ResultOrError<T>::Failed() const
    {
        return IsFailed;
    }
    template <typename T>
    IError::LogLevel ResultOrError<T>::GetLogLevel() const
    {
        return Level;
    }
}

#endif