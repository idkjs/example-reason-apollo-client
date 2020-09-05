module QueryResult = ApolloClient.Types.QueryResult;
module Cache = ApolloClient.Cache;

module FetchResult = ApolloClient.Types.FetchResult;
module TodoItem = Fragments.TodoItem;

// module TodoQuery =
[%graphql
  {|
    query TodoQuery($id:ID!) {
      todoItem:node(id: $id) {

        ...TodoItem
      }
    }
  |}
];
module UpdateMutation = [%graphql
  {|
  mutation SingleTodoUpdateMutation($input: UpdateTodoItemInput!) {
    updateTodoItem(input: $input) {
      updatedTodoItem {
        id
        text
        completed
      }
    }
  }
|}
];
// type t_updateTodoItem_updatedTodoItem = {
//   __typename: string,
//   id: string,
//   text: string,
//   completed: option(bool),
// };
[@react.component]
let make = (~checked, ~todoItem as todoItemRef) => {
  let (updateTodo, updateTodoResult) = UpdateMutation.use();
  switch (TodoQuery.use({id: todoItemRef})) {
  | {loading: true} => "Loading..."->React.string
  // | {error: Some(_error)} => "Error loading data"->React.string
  | {data: Some({todoItem: Some({todoItem})}), error} =>
    //  let todoItem = switch({id,text,completed}){
    //    |(Some(id), Some(text),Some(completed)) =>
    //    {id,text,completed}
    //    |(_,_,_)=> None
    //  };
    // let (completed,id,text) = todoItem;
    <div>
      <li
        className={
          switch (todoItem.completed) {
          | Some(true) => "completed"
          | Some(false)
          | None => ""
          }
        }>
        <div className="form-check">
          <label className="form-check-label">
            <input
              className="checkbox"
              type_="checkbox"
              checked
              onChange={_ => {
                // let completed =
                //   !Belt.Option.getWithDefault(todoItem.completed, false);
                updateTodo(
                  ~optimisticResponse=
                    _variables =>
                      {
                        updateTodoItem:
                          Some({
                            updatedTodoItem: {
                              id: todoItem.id,
                              completed:
                                !
                                  Belt.Option.getWithDefault(
                                    todoItem.completed,
                                    false,
                                  ),
                              text: todoItem.text,
                            },
                            __typename: todoItem.__typename,
                          }),
                      },
                  ~update=
                    (cache, {data}) =>
                      switch (data) {
                      | Some({data}) =>
                        /**
                   * Apollo docs use cache.modify, but it's not typesafe. I recommend some
                   * combination of readQuery / writeQuery / writeFragment
                   */
                        Js.log2("mutate.update To-Do: ", todoItem);
                        let _unusedRef =
                          cache->Cache.writeFragment(
                            ~fragment=(module Fragments.TodoItem),
                            ~data={
                              __typename: todoItem.__typename,
                              id: todoItem.id,
                              completed: None,
                              text: todoItem.text,
                            },
                            (),
                          );
                        let _unusedRef =
                          cache->Cache.writeQuery(
                            ~query=(module TodosQuery),
                            ~data={
                              node: [|
                                {
                                  __typename: todo.__typename,
                                  id: "writeQueryToDo",
                                  completed: None,
                                  text: "To-Do from writeQuery",
                                },
                              |],
                            },
                            (),
                          );
                        ();
                      | None => ()
                      },
                )
                ->ignore
              }}
            />
            {React.string(todoItem.text)}
          </label>
        </div>
        "There are "->React.string
        // {todoItem->Belt.Array.length->React.int}
        " To-Dos"->React.string
      </li>
    </div>

  | {loading: false, data: None} =>
    <p> "Error loading data"->React.string </p>
  | _ => <p> "Error loading data"->React.string </p>
  };
};
