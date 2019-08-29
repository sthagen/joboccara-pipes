#ifndef PIPES_UNZIP_HPP
#define PIPES_UNZIP_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/meta.hpp"

#include <tuple>
#include <type_traits>

namespace pipes
{

template<typename... TailPipelines>
class unzip_pipeline : public pipeline_base<unzip_pipeline<TailPipelines...>>
{
public:
    template<typename Tuple>
    void onReceive(Tuple&& values)
    {
        detail::for_each2([](auto&& value, auto&& tailPipe) { send(tailPipe, value); }, FWD(values), tailPipes_);
    }
    
    explicit unzip_pipeline(TailPipelines... tailPipes) : tailPipes_(tailPipes...) {}
    
private:
    std::tuple<TailPipelines...> tailPipes_;
};
    
template<typename... TailPipelines>
unzip_pipeline<TailPipelines...> unzip(TailPipelines... tailPipes)
{
    return unzip_pipeline<std::decay_t<TailPipelines>...>(tailPipes...);
}
    
} // namespace pipes

#endif /* PIPES_UNZIP_HPP */
