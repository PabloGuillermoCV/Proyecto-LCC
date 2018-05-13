// Reference to object provided by pengines.js library which interfaces with Pengines server (Prolog-engine)
// by making query requests and receiving answers.
var pengine;
// Bidimensional array representing grid configuration.
var gridData;
// Bidimensional array with grid cells elements.
var cellElems = null;
// Number of turns
var turns = 0;
// Reference to element displaying turns
var turnsElem;
// Variable global que mantiene la grilla actual
var currentGrid = 1;
// Variable que se encarga del manejo de la deshabilitacion del cambio de grilla
var firstMove = false;

/**
 * Representation of color enumerate as object where each property key defines an enum value (a color), and the
 * associated property value (the string) defines the color name.
 *
 * Values: RED, VIOLET, PINK, GREEN, BLUE, YELLOW
 */
const colors = Object.freeze({
    RED : "red",
    VIOLET : "violet",
    PINK : "pink",
    GREEN : "green",
    BLUE : "blue",
    YELLOW : "yellow"
});

/*
 * Returns the Prolog representation of the received color
 */
function colorToProlog(color) {
    return colors[color].charAt(0);
}

/*
 * Returns the color in colors enum associated to pColor, in Prolog representation.
 */
function colorFromProlog(pColor) {
    for (let color in colors) {
        if (colorToProlog(color) == pColor)
            return color;
    }
    return null;
}

/*
 * Returns the CSS representation of the received color.
 */
function colorToCss(color) {
    return colors[color];
}

/**
* Initialization function. Requests to server, through pengines.js library, 
* the creation of a Pengine instance, which will run Prolog code server-side.
*/
function init() {
    turnsElem = document.getElementById("turnsNum");
    pengine = new Pengine({
        server: "http://localhost:3030/pengine",
        application: "proylcc",
        oncreate: handleCreate,
        onsuccess: handleSuccess,
        destroy: false
    });

    var buttonsPanelElem = document.getElementById("buttonsPanel"); //Menu de seleccion de colores normal.
    var buttonsPanelAyudaBasica = document.getElementById("buttonsPanelAyudaBasica"); //Menu de los botones de ayuda basica.
	var buttonsPanelAyudaExtendida = document.getElementById("buttonsPanelAyudaExtendida"); //Menu de los botones de ayuda extendida.
	var buttonPanelGrid = document.getElementById("buttonPanelGrid"); //Panel para el boton para cambiar de grilla.
	var buttonGird = document.getElementById("buttonGrid"); //Boton para cambiar de grilla.

    for (let color in colors) {
        var buttonElem = document.createElement("button");
        buttonElem.className = "colorBtn";
        buttonElem.style.backgroundColor = colorToCss(color);
        buttonElem.addEventListener("click", function(e) {
            handleColorClick(color);
        });
        buttonsPanelElem.appendChild(buttonElem);
    }
	
	//HABRIA QUE MODIFICAR esto para que cada "boton" sea una etiqueta donde le podamos poner el resultado
	for(let color in colors){
        var buttonElem = document.createElement("button");
        buttonElem.className = "colorBtn";
        buttonElem.style.backgroundColor = colorToCss(color);
        buttonsPanelAyudaBasica.appendChild(buttonElem);
    }
	var buttonAyuda = document.createElement("button");
	buttonAyuda.className = "ayuda1Btn";
    buttonAyuda.innerHTML = "Ayuda Basica";
	buttonAyuda.addEventListener("click", function(e) { handleColorAyudaBasica (); });
	buttonsPanelAyudaBasica.appendChild(buttonAyuda);
	
	//HABRIA QUE MODIFICAR esto para que cada "boton" sea una etiqueta donde le podamos poner el resultado
    for(let color in colors){
        var buttonElem = document.createElement("button");
        buttonElem.className = "colorBtn";
        buttonElem.style.backgroundColor = colorToCss(color);
        //buttonElem.innerHTML = "algo"; para modificar el texto del botón 
        buttonsPanelAyudaExtendida.appendChild(buttonElem);
    }
	var buttonAyuda = document.createElement("button");
	buttonAyuda.className = "ayuda2Btn";
    buttonAyuda.innerHTML = "Ayuda Extendida";
	buttonAyuda.addEventListener("click", function(e) { handleColorAyudaExtendida(); });
	buttonsPanelAyudaExtendida.appendChild(buttonAyuda);
	
	buttonGrid.addEventListener("click", function(e) { handleCambioGrilla(); });
	buttonPanelGrid.appendChild(buttonGrid);
}

/**
 * Callback for Pengine server creation
 */
function handleCreate() {
    pengine.ask("grid(" + currentGrid + ", Grid)");
}

/**
 * Callback for successful response received from Pengines server
 * Aquí es donde se estaria modificando la Grilla, cada vez que Prolog tira "True"
 */
function handleSuccess(response) {
    gridData = response.data[0].Grid;
    if (cellElems == null)
        createGridElems(gridData.length, gridData[0].length);
    for (let row = 0; row < gridData.length; row++)
        for (let col = 0; col < gridData[row].length; col++)
            cellElems[row][col].style.backgroundColor = colorToCss(colorFromProlog(gridData[row][col]));
    turnsElem.innerHTML = turns;
}

/**
 * Create grid cells elements
 */
function createGridElems(numOfRows, numOfCols) {
    var gridElem = document.getElementById("grid");
    cellElems = [];
    for (let row = 0; row < numOfRows; row++) {
        cellElems[row] = [];
        for (let col = 0; col < numOfCols; col++) {
            var cellElem = document.createElement("div");
            gridElem.appendChild(cellElem);
            cellElems[row][col] = cellElem;
        }
    }
}

/**
 * Handler for color click. Ask query to Pengines server.
 */
function handleColorClick(color) {
    var s = "flick(" + Pengine.stringify(gridData) + "," + colorToProlog(color) + ",Grid)";
    pengine.ask(s);
	turns++;
	if (turns > 0) {
		document.getElemntById("buttonGrid").setAttribute("disabled","disabled"); //otra forma de hacer lo mismo, tampoco funciona...
	}
}

/**
* Handler para cambiar de grilla.
*/
function handleCambioGrilla () {
	currentGrid = currentGrid + 1;
	if (currentGrid == 5) {currentGrid = 1;}
	pengine.ask("grid(" + currentGrid + ", Grid)");
}

/**
* Handler para la ayuda basica.
* El metodo llama a la ayuda basica 1 vez por color para tener los 6 valores numericos.
*/
function handleColorAyudaBasica () {
	var s;
	for (let color in colors) {
        s = "ayudaBasica(" + Pengine.stringify(gridData) + "," + colorToProlog(color) + ",Res)";
		pengine.ask(s);
    }
}

/**
* Handler para la ayuda extendida.
* El metodo llama a la ayuda extendida 1 vez por color para tener los 6 valores numericos.
*/
function handleColorAyudaExtendida () {
	var s;
	for (let color in colors) {
        s = "ayudaExtendida(" + Pengine.stringify(gridData) + "," + colorToProlog(color) + ",Res)";
		pengine.ask(s);
    }
}

/**
* Call init function after window loaded to ensure all HTML was created before
* accessing and manipulating it.
*/
window.onload = init;