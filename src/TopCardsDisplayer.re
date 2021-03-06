/**
 * Here we define a fragment, telling Relay that if this component is to be
 * rendered, it needs the following fields on the type SiteStatistics.
 *
 * This is a pretty basic fragment, there are more complex ones you can have
 * a look at in other components.
 */
// module SiteStatisticsFragment = [%graphql
//   {|
//   fragment TopCardsDisplayer_siteStatistics on SiteStatistics {
//     weeklySales
//     weeklyOrders
//     currentVisitorsOnline
//   }
// |}
// ];
module QueryResult = ApolloClient.Types.QueryResult;
module StatisticsQuery = [%graphql
  {|
    query StatisticsQuery {
        siteStatistics {
            ...Statistics
        }
    }
|}
];
module CurrentVisitorsSubscription = [%graphql
  {|
  subscription TopCardsDisplayer_currentVisitorsOnline_Subscription {
    siteStatisticsUpdated {
      currentVisitorsOnline
    }
  }
|}
];

/**
 * A few things to note about the following component:
 * - I rename siteStatistics to siteStatisticsRef, which is purely because I
 *   feel it's more evident what it is (and that I want to use siteStatistics
 *   as name for the data I extract via the fragment) and is in no way
 *   required.
 * - [%graphql] autogenerates a `use` hook, which takes the object holding
 *   the fragment ref and extracts the data. Thanks to the superpowers of inference,
 *   we don't really need to annotate anything here for it to be 100% type safe.
 *
 */
[@react.component]
let make = (~siteStatistics as siteStatisticsRef) => {
//   let queryResult = StatisticsQuery.use(siteStatisticsRef);

  //   let environment = ReasonRelay.useEnvironmentFromContext();
  //   React.useEffect0(() => {
  //     let subscription =
  //       CurrentVisitorsSubscription.subscribe(
  //         ~environment,
  //         ~variables=(),
  //         ~onNext=
  //           response => {
  //             switch (response.siteStatisticsUpdated) {
  //             | Some(siteStatisticsUpdated) =>
  //               /* Console-logging the response for demo purposes
  //                  Note that the store (and thus the UI) gets updated "automatically" */
  //               Js.log2(
  //                 "Subscription response - current visitors online: ",
  //                 siteStatisticsUpdated.currentVisitorsOnline,
  //               )
  //             | None => ()
  //             }
  //           },
  //         (),
  //       );

  //     Some(() => ReasonRelay.Disposable.dispose(subscription));
  //   });
//   React.useEffect0(() => {
//     queryResult->QueryResult.subscribeToMore(
//       ~subscription=(module CurrentVisitorsSubscription),
//       ~updateQuery=
//         (previous, {subscriptionData: {data: {siteStatisticsUpdated}}}) => {
//           let count =
//             siteStatisticsUpdated
//             ->Belt.Option.map(stats => stats.currentVisitorsOnline)
//             ->Belt.Option.getWithDefault(0)
//             ->string_of_int;
//             count
//         //   siteStatisticsUpdated;
//         },
//       // ();
//       //   {
//       //     siteStatisticsUpdated:
//       //       Belt.Array.concat(
//       //         previous.todos,
//       //         [|
//       //           {
//       //             __typename: "TodoItem",
//       //             id: "subscribeToMoreTodo",
//       //             completed: None,
//       //             text: "Hello, " ++ count ++ " vistors online",
//       //           },
//       //         |],
//       //       ),
//       //   };
//     );
//     None;
//   });
  <div className="row">
    {switch (StatisticsQuery.use(siteStatisticsRef)) {
     | {loading: true, data: None} => <p> "Loading"->React.string </p>
     | {loading: false, data: Some({siteStatistics}), error: None} =>
       <>
         <div className="col-md-4 stretch-card grid-margin">
           <EmphasizedCard
             title="Weekly Sales"
             subText={"$ " ++ Js.Float.toString(siteStatistics.weeklySales)}
             icon=EmphasizedCard.ChartLine
             variant=EmphasizedCard.Red
           />
         </div>
         <div className="col-md-4 stretch-card grid-margin">
           <EmphasizedCard
             title="Weekly Orders"
             subText={string_of_int(siteStatistics.weeklyOrders)}
             icon=EmphasizedCard.Bookmark
             variant=EmphasizedCard.Blue
           />
         </div>
         <div className="col-md-4 stretch-card grid-margin">
           <EmphasizedCard
             title="Current Visitors Online"
             subText={string_of_int(siteStatistics.currentVisitorsOnline)}
             icon=EmphasizedCard.Diamond
             variant=EmphasizedCard.Green
           />
         </div>
       </>
     | {error} =>
       <>
         "Error loading data"->React.string
         {switch (error) {
          | Some(error) => React.string(": " ++ error.message)
          | None => React.null
          }}
       </>
     }}
  </div>;
};
