open Types;

module Fragment = [%graphql
  {|
  fragment Statistics on SiteStatistics @bsRecord {
    weeklySales
    weeklyOrders
    currentVisitorsOnline
  }
|}
];

[@react.component]
let make = (~statistics) => {
  <div className="row">
    <div className="col-md-4 stretch-card grid-margin">
      <EmphasizedCard
        title="Weekly Sales"
        subText={"$ " ++ Js.Float.toString(statistics.weeklySales)}
        icon=EmphasizedCard.ChartLine
        variant=EmphasizedCard.Red
      />
    </div>
    <div className="col-md-4 stretch-card grid-margin">
      <EmphasizedCard
        title="Weekly Orders"
        subText={string_of_int(statistics.weeklyOrders)}
        icon=EmphasizedCard.Bookmark
        variant=EmphasizedCard.Blue
      />
    </div>
    <div className="col-md-4 stretch-card grid-margin">
      <EmphasizedCard
        title="Current Visitors Online"
        subText={string_of_int(statistics.currentVisitorsOnline)}
        icon=EmphasizedCard.Diamond
        variant=EmphasizedCard.Green
      />
    </div>
  </div>;
};
