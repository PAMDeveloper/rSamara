#
# This is a Shiny web application. You can run the application by clicking
# the 'Run App' button above.
#
# Find out more about building applications with Shiny here:
#
#    http://shiny.rstudio.com/
#

library(shiny)
library(recomeristem)
library(ggplot2)

meteo = recomeristem::getMeteo_from_files("D:/Samples/ecomeristem_og_testSample")
params = recomeristem::getParameters_from_files("D:/Samples/ecomeristem_og_testSample")
result = recomeristem::rcpp_run_from_dataframe(params, meteo)[[1]]

# Define UI for application that draws a histogram
ui <- pageWithSidebar(
  headerPanel('Examples of DataTables'),
  sidebarPanel(
    tabsetPanel(
      tabPanel('mtcars',
               dataTableOutput("mytable2")),
      tabPanel('iris',
               dataTableOutput("mytable3")),
      tabPanel(
        'variables',
        fluidPage(
          checkboxGroupInput('show_vars',
                           'Columns in result to show:',
                           names(result),
                           selected = names(result))
        )
      )
    )
  ),
  mainPanel(
    tabPanel('diamonds',
             dataTableOutput("mytable1"))
    # 
    
    # helpText('For the diamonds data, we can select variables
    #          to show in the table; for the mtcars example, we
    #          use orderClasses = TRUE so that sorted columns 
    #          are colored since they have special CSS classes 
    #          attached; for the iris data, we customize the 
    #          length menu so we can display 5 rows per page.')
  )
)

# Define server logic required to draw a histogram
server <- function(input, output) {
  
  # a large table, reative to input$show_vars
  # customize the length drop-down menu; display 5 rows per page by default
  output$mytable1 = renderDataTable({
    result[, input$show_vars, drop = FALSE]
  })
  
  # sorted columns are colored now because CSS are attached to them
  output$mytable2 = renderDataTable({
    meteo
  }, options = list(orderClasses = TRUE))
  
  # customize the length drop-down menu; display 5 rows per page by default
  output$mytable3 = renderDataTable({
    params
  }, options = list(lengthMenu = c(5, 30, 50), pageLength = 25))
  
}

# Run the application 
shinyApp(ui = ui, server = server)

