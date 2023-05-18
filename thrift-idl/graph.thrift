namespace cpp graph_rpc

enum QueryType {
    ADD_EDGE = 1,
    REMOVE_EDGE = 2,
    DISTANCE = 3
}

enum CategoryType { 
    MUTATION = 1, 
    DISTANCE = 2 
}

struct Query {
    1: QueryType type,
    2: i32 u,
    3: i32 v,
}

service GraphService {
    list<i32> sendQueryBatch(1: string clientName, 2: list<Query> queryBatch)
}
