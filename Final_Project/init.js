// Global variables
var scene, renderer, camera, controls;
var container;
var camera, scene, renderer;
var cameraCube, sceneCube;
var mesh, lightMesh, geometry;
var spheres = [];
var directionalLight, pointLight;
var mouseX = 0;
var mouseY = 0;
var sound_effect;
var sound_effect2;
var path = "images/beach/";
var format = '.jpg';
var projector, mouse = new THREE.Vector2(), closestIntersection;
var raycaster;

var coolmode = false;
var urls = [
	path + 'posx' + format, path + 'negx' + format,
	path + 'posy' + format, path + 'negy' + format,
	path + 'posz' + format, path + 'negz' + format
];
var loader = new THREE.CubeTextureLoader();
var skybox = loader.load(urls);




function reset() {
	for ( var i = scene.children.length - 1; i >= 0 ; i -- ) {
		var obj = scene.children[i];
		scene.remove(obj);
		obj.geometry.dispose();
		obj.material.dispose();
	}
	this.scene = null;
	scene = new Physijs.Scene;
	scene.setGravity(new THREE.Vector3(0,0,0));
	sceneCube = new Physijs.Scene;
	var shader = THREE.FresnelShader;
	var geometry = new THREE.SphereBufferGeometry( 100, 32, 16 );

	// SKYBOX
	skybox.format = THREE.RGBFormat;
	skybox = loader.load(urls);
	var shader = THREE.ShaderLib[ "cube" ];
	shader.uniforms[ "tCube" ].value = skybox;
	var material = Physijs.createMaterial(
		new THREE.ShaderMaterial( {
			fragmentShader: shader.fragmentShader,
			vertexShader: shader.vertexShader,
			uniforms: shader.uniforms,
			side: THREE.BackSide
		} )
	);
	var mesh = new Physijs.BoxMesh( new THREE.BoxGeometry( 100000, 100000, 100000 ), material );
	mesh.__dirtyPosition = true;
	sceneCube.add( mesh );
	
	spheres = [];

	// BUBBLE SHADERS
	var shader = THREE.FresnelShader;
	var uniforms = THREE.UniformsUtils.clone( shader.uniforms );
	uniforms[ "tCube" ].value = skybox;
	var parameters = { fragmentShader: shader.fragmentShader, vertexShader: shader.vertexShader, uniforms: uniforms };
	var material = Physijs.createMaterial(
		new THREE.ShaderMaterial( parameters )
	);

	for ( var i = 0; i < 50; i ++ ) {
		var mesh = new Physijs.SphereMesh( geometry, material );
		mesh.position.x = Math.random() * 4000 - 2000;
		mesh.position.y = Math.random() * 4000 - 2000;
		mesh.position.z = Math.random() * 4000 - 2000;
		mesh.scale.x = mesh.scale.y = mesh.scale.z = Math.random() * 5 + 1;
		mesh.__dirtyPosition = true;

		scene.add( mesh );
		spheres.push( mesh );
		if (!coolmode) {
			spheres[i].setLinearVelocity({x: (Math.round(Math.random()) * 2 - 1) * 400,
										  y: (Math.round(Math.random()) * 2 - 1) * 400,
										  z: (Math.round(Math.random()) * 2 - 1) * 400});
			spheres[i].setAngularVelocity(new THREE.Vector3(Math.random() * 1000,
										  Math.random() * 1000, Math.random() * 1000));
		}	
	}


	// CAMERA
	camera = new THREE.PerspectiveCamera( 60, window.innerWidth / window.innerHeight, 1, 100000 );
	camera.position.set(0, 0, 4000);
	cameraCube = new THREE.PerspectiveCamera( 60, window.innerWidth / window.innerHeight, 1, 100000 );
	controls = new THREE.OrbitControls( camera );
	controls.addEventListener( 'change', render );


	render();
}


function toggleBeach() {

	path = "images/beach/";
	urls = [
		path + 'posx' + format, path + 'negx' + format,
		path + 'posy' + format, path + 'negy' + format,
		path + 'posz' + format, path + 'negz' + format
	];
	reset();
}

function toggleJack() {
	path = "images/"
	urls = [
		path + 'jack1' + format, path + 'jack1' + format,
		path + 'jack1' + format, path + 'jack1' + format,
		path + 'jack1' + format, path + 'jack1' + format
	];
	reset();
}

function toggleRoom() {
	path = "images/room/";
	urls = [
		path + 'posx' + format, path + 'negx' + format,
		path + 'posy' + format, path + 'negy' + format,
		path + 'posz' + format, path + 'negz' + format
	];
	reset();
}

function toggleWater() {
	path = "images/whirlpool/";
	urls = [
		path + 'posx' + format, path + 'negx' + format,
		path + 'posy' + format, path + 'negy' + format,
		path + 'posz' + format, path + 'negz' + format
	];
	reset();
}

function toggleSpace() {
	path = "images/horizon/";
	urls = [
		path + 'posx' + format, path + 'negx' + format,
		path + 'posy' + format, path + 'negy' + format,
		path + 'posz' + format, path + 'negz' + format
	];
	reset();
}


function toggleRemco() {
	path = "images/remco/";
	urls = [
		path + 'posx' + format, path + 'negx' + format,
		path + 'posy' + format, path + 'negy' + format,
		path + 'posz' + format, path + 'negz' + format
	];
	reset();
}


function toggleKitti() {
	path = "images/";
	urls = [
		path + 'kitti' + format, path + 'kitti' + format,
		path + 'kitti' + format, path + 'kitti' + format,
		path + 'kitti' + format, path + 'kitti' + format
	];
	reset();
}

function togglePsych1() {
	path = "images/";
	urls = [
		path + 'psych1' + format, path + 'psych1' + format,
		path + 'psych1' + format, path + 'psych1' + format,
		path + 'psych1' + format, path + 'psych1' + format
	];
	reset();
}

function togglePsych2() {
	path = "images/";
	urls = [
		path + 'psych2' + format, path + 'psych2' + format,
		path + 'psych2' + format, path + 'psych2' + format,
		path + 'psych2' + format, path + 'psych2' + format
	];
	reset();
}


function togglePsych3() {
	path = "images/";
	urls = [
		path + 'psych3' + format, path + 'psych3' + format,
		path + 'psych3' + format, path + 'psych3' + format,
		path + 'psych3' + format, path + 'psych3' + format
	];
	reset();
}

function superCoolMode() {

	if (!coolmode) {
		alert('you\'ve activated super kewl mode!!!!!11!1 8-\)');
		coolmode = true;
		document.getElementById("glasses-icon").style.color = "black";
	}
	else {
		coolmode = false;
		document.getElementById("glasses-icon").style.color = "white";
	}
	reset();
}

window.onload = function () {

	// WEBGL CHECK
	if( Detector.webgl ) {
		renderer = new THREE.WebGLRenderer( { antialias: true } );
	}
	// SUBSTITUTE IF NOT AVAILABLE
	else {
		renderer = new THREE.CanvasRenderer();
	}
	// RENDERER SETTINGS 
	renderer.setSize( window.innerWidth, window.innerHeight );
	renderer.setPixelRatio( window.devicePixelRatio );
	renderer.autoClear = false;

	container = document.createElement( 'container' );
	document.body.appendChild( container );
	container.appendChild( renderer.domElement );

	// INIT RAYCASTER FOR POPPING
	projector = new THREE.Projector();
	raycaster = new THREE.Raycaster();

	// INIT PHYSIJS
	Physijs.scripts.worker = '/physijs/physijs_worker.js';
    Physijs.scripts.ammo = '/physijs/ammo.js';

	// CREATE SCENE
	scene = new Physijs.Scene;
	scene.setGravity(new THREE.Vector3(0,0,0));
	sceneCube = new Physijs.Scene;
	var geometry = new THREE.SphereBufferGeometry( 100, 32, 16 );
	var path = "images/";
	var format = '.jpg';

	// SOUND EFFECTS
	sound_effect = new Audio("pop.mp3");
	sound_effect2 = new Audio("hi_pop.mp3")

	// SKYBOX
	skybox.format = THREE.RGBFormat;
	var shader = THREE.ShaderLib[ "cube" ];
	shader.uniforms[ "tCube" ].value = skybox;
	var material = Physijs.createMaterial(
		new THREE.ShaderMaterial( {
			fragmentShader: shader.fragmentShader,
			vertexShader: shader.vertexShader,
			uniforms: shader.uniforms,
			side: THREE.BackSide
		} )
	);
	var mesh = new Physijs.BoxMesh( new THREE.BoxGeometry( 100000, 100000, 100000 ), material );
	mesh.__dirtyPosition = true;
	sceneCube.add( mesh );


	// BUBBLE SHADERS
	var shader = THREE.FresnelShader;
	var uniforms = THREE.UniformsUtils.clone( shader.uniforms );
	uniforms[ "tCube" ].value = skybox;
	var parameters = { fragmentShader: shader.fragmentShader, vertexShader: shader.vertexShader, uniforms: uniforms };
	var material = Physijs.createMaterial(
		new THREE.ShaderMaterial( parameters )
	);

	for ( var i = 0; i < 50; i ++ ) {
		var mesh = new Physijs.SphereMesh( geometry, material );
		mesh.position.x = Math.random() * 4000 - 2000;
		mesh.position.y = Math.random() * 4000 - 2000;
		mesh.position.z = Math.random() * 4000 - 2000;
		mesh.scale.x = mesh.scale.y = mesh.scale.z = Math.random() * 5 + 1;
		mesh.__dirtyPosition = true;

		scene.add( mesh );
		spheres.push( mesh );

		if (!coolmode) {
			spheres[i].setLinearVelocity({x: (Math.round(Math.random()) * 2 - 1) * 400,
										  y: (Math.round(Math.random()) * 2 - 1) * 400,
										  z: (Math.round(Math.random()) * 2 - 1) * 400});
			spheres[i].setAngularVelocity(new THREE.Vector3(Math.random() * 1000,
										  Math.random() * 1000, Math.random() * 1000));
		}
	}

	// CAMERA
	camera = new THREE.PerspectiveCamera( 60, window.innerWidth / window.innerHeight, 1, 100000 );
	camera.position.set(0, 0, 4000);
	cameraCube = new THREE.PerspectiveCamera( 60, window.innerWidth / window.innerHeight, 1, 100000 );

	// CAMERA CONTROL
	controls = new THREE.OrbitControls( camera );
	controls.addEventListener( 'change', render );

	window.addEventListener( 'resize', onWindowResize, false );
	document.addEventListener( 'mousemove', onDocumentMouseMove, false );

	// ANIMATE
	animate();
}



function animate() 
{
    requestAnimationFrame( animate );
	render();
}

function render() {
	scene.simulate();

	camera.lookAt( scene.position );
	cameraCube.rotation.copy( camera.rotation );
	renderer.render( sceneCube, cameraCube );
	renderer.render( scene, camera );
	
}


/* window functions */
function onWindowResize() {
	camera.aspect = window.innerWidth / window.innerHeight;
	camera.updateProjectionMatrix();
	cameraCube.aspect = window.innerWidth / window.innerHeight;
	cameraCube.updateProjectionMatrix();
	renderer.setSize( window.innerWidth, window.innerHeight );
}

function onDocumentMouseMove( event ) 
{
	event.preventDefault();
	mouse.x = ( event.clientX / window.innerWidth ) * 2 - 1;
	mouse.y = - ( event.clientY / window.innerHeight ) * 2 + 1;
	updateIntersections();
}


function cloneBubble(bubble){
    var newBubble = new Physijs.SphereMesh(bubble.geometry, bubble.material);
    return newBubble;
}

function updateIntersections() {
	scene.updateMatrixWorld();
	raycaster.setFromCamera(mouse,camera);
	var intersects = raycaster.intersectObjects( scene.children );

	if ( intersects.length > 0 ) {
		if ( closestIntersection != intersects[ 0 ].object ) {


			closestIntersection = intersects[ 0 ].object;
			sound_effect.play();

			if (coolmode) var dx = 0.1;
			else var dx = closestIntersection.scale.x * 100;
			var old_len = spheres.length;

			var shader = THREE.FresnelShader;
			var geometry = new THREE.SphereBufferGeometry( 100, 32, 16 );
			shader.uniforms[ "tCube" ].value = skybox;
			var uniforms = THREE.UniformsUtils.clone( shader.uniforms );
			var parameters = { fragmentShader: shader.fragmentShader, vertexShader: shader.vertexShader, uniforms: uniforms };
			var material = Physijs.createMaterial(
				new THREE.ShaderMaterial( parameters )
			);

			var temp1 = cloneBubble(closestIntersection);
			temp1.position.x = closestIntersection.position.x + dx;
			temp1.position.y = closestIntersection.position.y + dx;
			temp1.position.z = closestIntersection.position.z + dx;
			temp1.scale.x = closestIntersection.scale.x / 1.3;
			temp1.scale.y = closestIntersection.scale.y / 1.3;
			temp1.scale.z = closestIntersection.scale.z / 1.3;
			temp1.__dirtyPosition = true;

			var temp2 = cloneBubble(closestIntersection);
			temp2.position.x = closestIntersection.position.x - dx;
			temp2.position.y = closestIntersection.position.y - dx;
			temp2.position.z = closestIntersection.position.z - dx;
			temp2.scale.x = closestIntersection.scale.x / 1.3;
			temp2.scale.y = closestIntersection.scale.y / 1.3;
			temp2.scale.z = closestIntersection.scale.z / 1.3;
			temp2.__dirtyPosition = true;
		
			var temp3 = cloneBubble(closestIntersection);
			temp3.position.x = closestIntersection.position.x + dx;
			temp3.position.y = closestIntersection.position.y - dx;
			temp3.position.z = closestIntersection.position.z + dx;
			temp3.scale.x = closestIntersection.scale.x / 1.3;
			temp3.scale.y = closestIntersection.scale.y / 1.3;
			temp3.scale.z = closestIntersection.scale.z / 1.3;
			temp3.__dirtyPosition = true;

			var temp4 = cloneBubble(closestIntersection);
			temp4.position.x = closestIntersection.position.x - dx;
			temp4.position.y = closestIntersection.position.y + dx;
			temp4.position.z = closestIntersection.position.z - dx;
			temp4.scale.x = closestIntersection.scale.x / 1.3;
			temp4.scale.y = closestIntersection.scale.y / 1.3;
			temp4.scale.z = closestIntersection.scale.z / 1.3;
			temp4.__dirtyPosition = true;
			
			scene.remove(closestIntersection);
			spheres.pop();
			if (temp1.scale.x > 1.0) {
				scene.add( temp1 );
				spheres.push( temp1 );
				sound_effect2.play();
				if (!coolmode) temp1.setLinearVelocity(new THREE.Vector3((Math.round(Math.random()) * 2 - 1) * 400, (Math.round(Math.random()) * 2 - 1) * 400, (Math.round(Math.random()) * 2 - 1) * 400));
			} 

			if (temp2.scale.x > 1.0) {
				scene.add( temp2 );
				spheres.push( temp2 );
				sound_effect2.play();
				if (!coolmode) temp2.setLinearVelocity(new THREE.Vector3((Math.round(Math.random()) * 2 - 1) * 400, (Math.round(Math.random()) * 2 - 1) * 400, (Math.round(Math.random()) * 2 - 1) * 400));
			}
			if (temp3.scale.x > 1.0) {
				scene.add( temp3 );
				spheres.push( temp3);
				sound_effect2.play();
				if (!coolmode) temp3.setLinearVelocity(new THREE.Vector3((Math.round(Math.random()) * 2 - 1) * 400, (Math.round(Math.random()) * 2 - 1) * 400, (Math.round(Math.random()) * 2 - 1) * 400));
			}

			if (temp4.scale.x > 1.0) {
				scene.add( temp4 );
				spheres.push( temp4 );
				sound_effect2.play();
				if (!coolmode) temp4.setLinearVelocity(new THREE.Vector3((Math.round(Math.random()) * 2 - 1) * 400, (Math.round(Math.random()) * 2 - 1) * 400, (Math.round(Math.random()) * 2 - 1) * 400));
			}

		}
	} else {
		closestIntersection = null;
	}	
}

