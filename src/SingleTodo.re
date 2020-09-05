// module QueryResult = ApolloClient.Types.QueryResult;
// module TodoItem = Fragments.TodoItem;

// // module TodoQuery =
// [%graphql
//   {|
//     query TodoQuery($id:ID!) {
//       todoItem:node(id: $id) {

//         ...TodoItem
//       }
//     }
//   |}
// ];

// module DeleteMutation = [%graphql
//   {|
//   mutation SingleTodoDeleteMutation($input: DeleteTodoItemInput!) {
//     deleteTodoItem(input: $input) {
//       deletedTodoItemId
//       clientMutationId
//     }
//   }
// |}
// ];

// module UpdateMutation = [%graphql
//   {|
//   mutation SingleTodoUpdateMutation($input: UpdateTodoItemInput!) {
//     updateTodoItem(input: $input) {
//       updatedTodoItem {
//         id
//         text
//         completed
//       }
//     }
//   }
// |}
// ];

// [@react.component]
// let make = (~checked, ~todoItem as todoItemRef) => {
//   // let queryResult = TodoQuery.use();
//   let todoItem = TodoQuery.use({id: todoItemRef});

//   <div>
//     {switch (todoItem) {
//      | {loading: true, data: None,_} => <p> "Loading"->React.string </p>
//      | {loading: true, data:Some({todoItem:None})} => <p> "Loading"->React.string </p>
//      | { data: Some({todoItem: Some({todoItem})}), error} =>
//        <li
//          className={
//            switch (todoItem.completed) {
//            | Some(true) => "completed"
//            | Some(false)
//            | None => ""
//            }
//          }>
//          <div className="form-check">
//            <label className="form-check-label">
//              <input
//                className="checkbox"
//                type_="checkbox"
//                checked
//                onChange={_ => {
//                  let completed =
//                    !Belt.Option.getWithDefault(todoItem.completed, false);
//                  UpdateMutation.useWithVariables(
//                    ~variables={
//                      input: {
//                        clientMutationId: None,
//                        id: todoItem.id,
//                        completed,
//                        text: todoItem.text,
//                      },
//                    },
//                    ~optimisticResponse={
//                      updateTodoItem:
//                        Some({
//                          updatedTodoItem:
//                            Some({
//                              id: todoItem.id,
//                              completed: Some(completed),
//                              text: todoItem.text,
//                            }),
//                        }),
//                    },
//                    (),
//                  )
//                  |> ignore;
//                }}
//              />
//              {React.string(todoItem.text)}
//            </label>
//          </div>
//          <i
//            onClick={_ =>
//              DeleteMutation.commitMutation(
//                ~environment=RelayEnv.environment,
//                ~variables={
//                  input: {
//                    clientMutationId: None,
//                    id: todoItem.id,
//                  },
//                },
//                ~updater=
//                  (store, _response) =>
//                    switch (
//                      store->ReasonRelay.RecordSourceSelectorProxy.get(
//                        ~dataId=todoItem.id->ReasonRelay.makeDataId,
//                      )
//                    ) {
//                    | Some(node) =>
//                      ReasonRelayUtils.(
//                        removeNodeFromConnections(
//                          ~store,
//                          ~node,
//                          ~connections=[
//                            {
//                              parentID: ReasonRelay.storeRootId,
//                              key: "TodoList_query_todosConnection",
//                              filters: None,
//                            },
//                          ],
//                        )
//                      )
//                    | None => ()
//                    },
//                (),
//              )
//              |> ignore
//            }
//            role="button"
//            className="remove mdi mdi-close-circle-outline"
//          />
//        </li>
//      | {loading: false, data: None} =>
//        <p> "Error loading data"->React.string </p>
//      }}
//   </div>;
// };
