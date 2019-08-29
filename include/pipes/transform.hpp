#ifndef PIPES_TRANSFORM_HPP
#define PIPES_TRANSFORM_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/FWD.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/helpers/invoke.hpp"

#include <type_traits>

namespace pipes
{

template<typename Function, typename TailPipeline>
class transform_pipeline : public pipeline_base<transform_pipeline<Function, TailPipeline>>
{
public:
    template<typename T>
    void onReceive(T&& input)
    {
        send(tailPipeline_, detail::invoke(function_.get(), FWD(input)));
    }

    explicit transform_pipeline(Function function, TailPipeline tailPipeline) : function_(function), tailPipeline_(tailPipeline) {}
    
private:
    detail::assignable<Function> function_;
    TailPipeline tailPipeline_;
};

template<typename Function>
class transform_pipe
{
public:
    template<typename Pipeline>
    auto plug_to_pipeline(Pipeline&& pipeline) const
    {
        return transform_pipeline<Function, std::remove_reference_t<Pipeline>>{function_, pipeline};
    }
    
    explicit transform_pipe(Function function) : function_(function){}

private:
    Function function_;
};

template<typename Function>
transform_pipe<std::decay_t<Function>> transform(Function&& function)
{
    return transform_pipe<std::decay_t<Function>>{function};
}

} // namespace pipes

#endif /* PIPES_TRANSFORM_HPP */
