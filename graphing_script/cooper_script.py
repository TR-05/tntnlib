import plotly.express as px
from plotly.graph_objs import *
import dash
from dash import dcc, html
from dash.dependencies import Input, Output
import pandas

app = dash.Dash(__name__) #
app.layout = html.Div(
    html.Div([
        html.H4('TrogLib', style={"color": "black"}),
        html.Div(id='live-update-text', style={'color': 'white'}),
        dcc.Graph(id='live-update-graph', style={'color': 'white'}),
        dcc.Interval(
            id='interval-component',
            interval=5000000, # in milliseconds
            n_intervals=0
        )
    ])
)

@app.callback(Output('live-update-graph', 'figure'),
              Input('interval-component', 'n_intervals'))
def update_graph_live(n):

    with open("data.csv", "r") as f:
        dataStarted = False
        for line in f:
            if "start" in line:
                dataStarted = True
                with open("cleanedData.csv", "w") as f2:
                    f2.write("")
                continue
            if dataStarted:
                # clear clean data file                
                with open("cleanedData.csv", "a") as f2:
                    f2.write(line)
    df = pandas.read_csv('cleanedData.csv', skiprows=0)
    df['time'] = range(len(df))
    print(df.dtypes)

    plot = px.line( df,
    x="time",
    y=[
        "leftTarget",
        "rightTarget",
        "leftAccel",
        "rightAccel",
        "leftVel",
        "rightVel",
    ],

    labels={
    "time": "Time(S)",
    },
    title="Troglib (cooper)",


    #paper_bgcolor='rgb(233,233,233)', # set the background colour
    markers=True)
    plot.update_layout(height=850, width=1912, 
    font_family="Gravitas One",
    font_color="white",
    font_size = 20,
    title_font_family="Gravitas One",
    title_font_color="white",
    legend_title_font_color="green",
    legend_font_size = 30,
    legend_bgcolor="rgba(31, 36, 36, 0.95)",
    legend_bordercolor="black",
    legend_borderwidth=2,
    legend_entrywidth=2,
    paper_bgcolor="rgba(40, 40, 40, 1.0)",
    #plot_bordercolor="black",
    #plot_borderwidth=2,
    plot_bgcolor="rgba(60, 60, 60, 1.0)",
     modebar = dict(bgcolor='rgba(31, 36, 36, 0.95)')  
    )
    # plot.uirevision = True
    return plot

if __name__ == '__main__':
    app.run_server(debug=True)