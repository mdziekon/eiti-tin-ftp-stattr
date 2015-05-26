#ifndef TIN_AGENT_SRC_MODELS_STATSGATHERERVISITOR
#define TIN_AGENT_SRC_MODELS_STATSGATHERERVISITOR

namespace tin { namespace agent { namespace models
{
    class StatsGatherer;
    
    namespace events
    {
        struct IncomingPacket;
        struct RequestPackets;
    }
    
    class StatsGathererVisitor
    {
        friend class tin::agent::models::StatsGatherer;
        
        tin::agent::models::StatsGatherer& statsGatherer;
        
        StatsGathererVisitor(StatsGatherer& statsGatherer);
    public:
        void visit(const tin::agent::models::events::IncomingPacket& event);
        void visit(const tin::agent::models::events::RequestPackets& event);
    };
}}}

#endif