#ifndef TIN_SUPERVISOR_SRC_MODELS_STATSVISITOR_HPP
#define TIN_SUPERVISOR_SRC_MODELS_STATSVISITOR_HPP

namespace tin { namespace supervisor { namespace models 
{
    class Stats;
    
    namespace events
    {
        struct ReceivePackets;
        struct RequestAnalytics;
    }

    class StatsVisitor
    {
        friend class tin::supervisor::models::Stats;

        tin::supervisor::models::Stats& stats;

        StatsVisitor(tin::supervisor::models::Stats& stats);
        
    public:
        void visit(tin::supervisor::models::events::ReceivePackets& event);
        void visit(tin::supervisor::models::events::RequestAnalytics& event);
    };
}}}

#endif